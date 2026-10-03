"""Emits C for edited trainers and splices it into src/data/trainers.h.

The guiding rule is minimum disturbance. An entry that was not edited is written
back as the exact bytes it was read as. Within an edited entry, individual fields
and party members that were not touched also keep their original bytes, so the
diff shows only what actually changed.
"""

from __future__ import annotations

import os
import shutil
import time
from datetime import datetime
from pathlib import Path

from .model import (
    MON_FIELDS,
    MON_INDENT,
    MOVE_INDENT,
    TRAINER_FIELDS,
    TRAINER_INDENT,
    Field,
    MonEntry,
    TrainerEntry,
)
from .parser import TrainerFile


def _ordered(names, specs) -> list[str]:
    """Keep the order fields already had; place genuinely new fields by spec."""
    seen = list(names)
    known = [n for n in seen if n in specs]
    unknown = [n for n in seen if n not in specs]
    return known + unknown


def _field_line(field: Field, indent: str) -> str:
    """Original bytes for untouched fields, freshly generated C for edited ones.

    Keeping `field.raw` matters beyond cosmetics: `.encounterMusic_gender` is
    written across two lines in this file, and `.dynamaxLevel` holds the symbol
    MAX_DYNAMAX_LEVEL rather than a number. Re-emitting either from the decoded
    value would change lines nobody asked to change.
    """
    if not field.dirty and field.raw:
        return field.raw
    return f"{indent}.{field.name} = {field.value_text},\n"


def _moves_block(moves: list[str]) -> str:
    if not moves:
        return ""
    out = f"{MON_INDENT}.moves = {{\n"
    for move in moves:
        out += f"{MOVE_INDENT}{move},\n"
    out += f"{MON_INDENT}}},\n"
    return out


def mon_is_dirty(mon: MonEntry) -> bool:
    return mon.dirty or any(f.dirty for f in mon.fields.values())


def emit_mon(mon: MonEntry) -> str:
    if not mon_is_dirty(mon) and mon.raw:
        return mon.raw

    out = f"{MON_INDENT}{{\n"
    for name in _ordered(mon.fields.keys(), MON_FIELDS):
        if name == "moves":
            out += _moves_block(mon.moves)
            continue
        out += _field_line(mon.fields[name], MON_INDENT)
    if "moves" not in mon.fields and mon.moves:
        out += _moves_block(mon.moves)
    out += f"{MON_INDENT}}},\n"
    return out


def trainer_is_dirty(entry: TrainerEntry) -> bool:
    return (
        entry.dirty
        or any(f.dirty for f in entry.fields.values())
        or any(mon_is_dirty(mon) for mon in entry.party)
    )


def emit_trainer(entry: TrainerEntry) -> str:
    if not trainer_is_dirty(entry) and entry.raw:
        return entry.raw

    out = entry.header
    for name in _ordered(entry.fields.keys(), TRAINER_FIELDS):
        if name == "party":
            out += entry.party_prefix
            for mon in entry.party:
                out += emit_mon(mon)
            out += entry.party_suffix
            continue
        out += _field_line(entry.fields[name], TRAINER_INDENT)
    out += entry.footer
    return out


def render_file(trainer_file: TrainerFile) -> str:
    """Rebuild the whole file: original bytes everywhere except edited entries."""
    chunks: list[str] = []
    cursor = 0
    for entry in sorted(trainer_file.entries, key=lambda e: e.span[0]):
        start, end = entry.span
        chunks.append(trainer_file.text[cursor:start])
        chunks.append(emit_trainer(entry))
        cursor = end
    chunks.append(trainer_file.text[cursor:])
    return "".join(chunks)


# --------------------------------------------------------------------------
# Factories for brand new entries
# --------------------------------------------------------------------------

def _make_field(name: str, value, specs) -> Field:
    spec = specs[name]
    text = spec.codec.encode(value)
    return Field(name=name, raw="", value_text=text, value=value, dirty=True)


def new_mon(species: str = "SPECIES_NONE", level: int = 5) -> MonEntry:
    """A party member matching the shape every mon in this file already has."""
    mon = MonEntry(dirty=True)
    for name, value in (
        ("species", species),
        ("gender", "TRAINER_MON_RANDOM_GENDER"),
        ("iv", [0, 0, 0, 0, 0, 0]),
        ("lvl", level),
        ("nature", "NATURE_HARDY"),
    ):
        mon.fields[name] = _make_field(name, value, MON_FIELDS)
    # `.dynamaxLevel = MAX_DYNAMAX_LEVEL` appears on all 2147 existing mons.
    mon.fields["dynamaxLevel"] = Field(
        name="dynamaxLevel", raw="", value_text="MAX_DYNAMAX_LEVEL", value=None, dirty=True
    )
    return mon


def new_trainer(key: str, difficulty: str = "DIFFICULTY_NORMAL") -> TrainerEntry:
    entry = TrainerEntry(
        key=key,
        difficulty=difficulty,
        span=(0, 0),
        raw="",
        header=f"    [{difficulty}][{key}] =\n    {{\n",
        footer="    },\n",
        party_prefix=f"{TRAINER_INDENT}.party = (const struct TrainerMon[])\n{TRAINER_INDENT}{{\n",
        party_suffix=f"{TRAINER_INDENT}}},\n",
        dirty=True,
    )
    for name, value in (
        ("trainerName", ""),
        ("trainerClass", "TRAINER_CLASS_PKMN_TRAINER_1"),
        ("trainerPic", "TRAINER_PIC_HIKER"),
        ("encounterMusic_gender", "TRAINER_ENCOUNTER_MUSIC_MALE"),
        ("doubleBattle", False),
        ("partySize", 0),
    ):
        entry.fields[name] = _make_field(name, value, TRAINER_FIELDS)
    entry.fields["party"] = Field(name="party", raw="", value_text="", value="", dirty=True)
    return entry


# --------------------------------------------------------------------------
# Safe writing
# --------------------------------------------------------------------------

def backup(path: Path, backup_dir: Path) -> Path:
    """Timestamped copy kept before any write. Never overwrites a prior backup."""
    backup_dir.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now().strftime("%Y%m%d-%H%M%S")
    target = backup_dir / f"{path.name}.{stamp}.bak"
    counter = 1
    while target.exists():
        target = backup_dir / f"{path.name}.{stamp}-{counter}.bak"
        counter += 1
    shutil.copy2(path, target)
    return target


def write_atomic(path: Path, text: str) -> None:
    """Write via a temp file in the same directory, then replace.

    A crash mid-write leaves the original file intact rather than truncated.
    """
    temp = path.with_suffix(path.suffix + f".tmp{os.getpid()}-{int(time.time() * 1000)}")
    try:
        temp.write_bytes(text.encode("utf-8"))
        os.replace(temp, path)
    finally:
        if temp.exists():
            temp.unlink(missing_ok=True)
