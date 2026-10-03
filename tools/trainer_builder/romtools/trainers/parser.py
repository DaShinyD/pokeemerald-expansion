"""Parses src/data/trainers.h into the internal model, recording byte spans.

This file is `#include`d directly into `gTrainers[DIFFICULTY_COUNT][TRAINERS_COUNT]`
in src/data.c, so it is a bare sequence of designated initialisers:

    [DIFFICULTY_NORMAL][TRAINER_SAWYER_1] =
    {
        .trainerName = _("SAWYER"),
        ...
        .party = (const struct TrainerMon[])
        {
            {
            .species = SPECIES_GEODUDE,
            ...
            },
        },
    },

The scanner is brace-aware rather than line-based, because several fields span
multiple lines (`.encounterMusic_gender`, `.party`, `.moves`). Anything that
cannot be parsed confidently raises ParseError with a line number instead of
being guessed at.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field as dc_field

from .model import (
    MON_FIELDS,
    TRAINER_FIELDS,
    Field,
    MonEntry,
    TrainerEntry,
)


class ParseError(Exception):
    def __init__(self, message: str, source: str = "", offset: int = 0):
        self.line = source.count("\n", 0, offset) + 1 if source else 0
        super().__init__(f"{message} (line {self.line})" if self.line else message)


_ENTRY_RE = re.compile(
    r"^[ \t]*\[(?P<difficulty>DIFFICULTY_\w+)\]\[(?P<key>\w+)\][ \t]*=[ \t]*\r?\n?[ \t]*\{",
    re.MULTILINE,
)
_FIELD_RE = re.compile(r"\.(\w+)[ \t]*=")


def _line_start(text: str, index: int) -> int:
    return text.rfind("\n", 0, index) + 1


def _end_of_line(text: str, index: int) -> int:
    newline = text.find("\n", index)
    return len(text) if newline < 0 else newline + 1


def _skip_quoted(text: str, index: int) -> int:
    quote = text[index]
    index += 1
    while index < len(text):
        if text[index] == "\\":
            index += 2
            continue
        if text[index] == quote:
            return index + 1
        index += 1
    raise ParseError("Unterminated string literal", text, index)


def _match_brace(text: str, open_index: int) -> int:
    """Index of the `}` closing the `{` at open_index."""
    depth = 0
    index = open_index
    while index < len(text):
        char = text[index]
        if char in "\"'":
            index = _skip_quoted(text, index)
            continue
        if char == "/" and text[index + 1: index + 2] == "*":
            close = text.find("*/", index + 2)
            index = len(text) if close < 0 else close + 2
            continue
        if char == "/" and text[index + 1: index + 2] == "/":
            index = _end_of_line(text, index)
            continue
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    raise ParseError("Unbalanced braces", text, open_index)


def _scan_value_end(text: str, index: int) -> int:
    """Index of the `,` terminating a field value, ignoring nested brackets."""
    depth = 0
    while index < len(text):
        char = text[index]
        if char in "\"'":
            index = _skip_quoted(text, index)
            continue
        if char in "({[":
            depth += 1
        elif char in ")}]":
            if depth == 0:
                return index  # closing brace of the enclosing block: last field had no comma
            depth -= 1
        elif char == "," and depth == 0:
            return index
        index += 1
    raise ParseError("Unterminated field value", text, index)


@dataclass
class _RawField:
    name: str
    raw: str
    value_text: str
    value_start: int
    value_end: int
    start: int
    end: int


def _parse_fields(text: str, body_start: int, body_end: int) -> list[_RawField]:
    """Walk `.name = value,` pairs at one nesting level, in source order."""
    fields: list[_RawField] = []
    cursor = body_start
    while True:
        match = _FIELD_RE.search(text, cursor, body_end)
        if not match:
            break
        value_start = match.end()
        while value_start < body_end and text[value_start] in " \t":
            value_start += 1
        value_end = _scan_value_end(text, value_start)
        start = _line_start(text, match.start())
        end = _end_of_line(text, value_end)
        fields.append(
            _RawField(
                name=match.group(1),
                raw=text[start:end],
                value_text=text[value_start:value_end].strip(),
                value_start=value_start,
                value_end=value_end,
                start=start,
                end=end,
            )
        )
        cursor = max(end, value_end + 1)
    return fields


def _decode(raw_field: _RawField, specs: dict, text: str) -> Field:
    spec = specs.get(raw_field.name)
    value = spec.codec.decode(raw_field.value_text) if spec else raw_field.value_text
    return Field(
        name=raw_field.name,
        raw=raw_field.raw,
        value_text=raw_field.value_text,
        value=value,
    )


def _parse_party(text: str, party_field: _RawField) -> tuple[str, str, list[MonEntry]]:
    """Split `.party = (const struct TrainerMon[]) { {...}, {...} },` into mons."""
    list_open = text.find("{", party_field.value_start, party_field.value_end + 1)
    if list_open < 0:
        raise ParseError("`.party` has no initialiser list", text, party_field.value_start)
    list_close = _match_brace(text, list_open)

    prefix = text[party_field.start:_end_of_line(text, list_open)]
    suffix = text[_line_start(text, list_close):party_field.end]

    mons: list[MonEntry] = []
    cursor = list_open + 1
    while True:
        mon_open = text.find("{", cursor, list_close)
        if mon_open < 0:
            break
        mon_close = _match_brace(text, mon_open)
        mon_start = _line_start(text, mon_open)
        mon_end = _end_of_line(text, mon_close)
        # include the trailing `,` after `}` if present
        comma = text.find(",", mon_close, mon_end)
        if comma >= 0:
            mon_end = _end_of_line(text, comma)

        mon = MonEntry(raw=text[mon_start:mon_end])
        for raw_field in _parse_fields(text, mon_open + 1, mon_close):
            decoded = _decode(raw_field, MON_FIELDS, text)
            mon.fields[decoded.name] = decoded
            if decoded.name == "moves":
                from .model import _dec_list

                mon.moves = _dec_list(raw_field.value_text)
        mons.append(mon)
        cursor = mon_end
    return prefix, suffix, mons


def parse_trainers(text: str) -> list[TrainerEntry]:
    """Parse the whole of src/data/trainers.h."""
    entries: list[TrainerEntry] = []
    for match in _ENTRY_RE.finditer(text):
        open_index = text.rindex("{", match.start(), match.end())
        close_index = _match_brace(text, open_index)

        start = _line_start(text, match.start())
        end = _end_of_line(text, close_index)
        comma = text.find(",", close_index, end)
        if comma >= 0:
            end = _end_of_line(text, comma)

        body_start = _end_of_line(text, open_index)
        footer_start = _line_start(text, close_index)

        entry = TrainerEntry(
            key=match.group("key"),
            difficulty=match.group("difficulty"),
            span=(start, end),
            raw=text[start:end],
            header=text[start:body_start],
            footer=text[footer_start:end],
            party_prefix="",
            party_suffix="",
        )

        for raw_field in _parse_fields(text, body_start, footer_start):
            if raw_field.name == "party":
                prefix, suffix, mons = _parse_party(text, raw_field)
                entry.party_prefix, entry.party_suffix, entry.party = prefix, suffix, mons
            decoded = _decode(raw_field, TRAINER_FIELDS, text)
            entry.fields[decoded.name] = decoded

        if "party" not in entry.fields:
            raise ParseError(f"{entry.key} has no `.party` field", text, start)
        entries.append(entry)

    if not entries:
        raise ParseError("No trainer entries found - is this src/data/trainers.h?")
    return entries


@dataclass
class TrainerFile:
    """The parsed file plus the original bytes, so unedited regions survive."""

    path: str
    text: str
    entries: list[TrainerEntry] = dc_field(default_factory=list)

    @classmethod
    def load(cls, path) -> "TrainerFile":
        from pathlib import Path

        raw = Path(path).read_bytes().decode("utf-8", errors="strict")
        return cls(path=str(path), text=raw, entries=parse_trainers(raw))

    def by_key(self, key: str, difficulty: str = "DIFFICULTY_NORMAL") -> TrainerEntry | None:
        for entry in self.entries:
            if entry.key == key and entry.difficulty == difficulty:
                return entry
        return None

    def keys(self) -> list[str]:
        return [entry.key for entry in self.entries]
