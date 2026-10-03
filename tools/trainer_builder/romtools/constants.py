"""Scrapes constant tables (species, moves, items, ...) live from the repository.

Nothing here is hardcoded: add SPECIES_FAKEMON to include/constants/species.h and
it shows up the next time the tool starts. Two source shapes are supported, both
of which this repository uses:

  * `#define NAME value`  - species, moves, items, abilities, natures, AI flags,
                            trainer classes/pics/music, trainer IDs
  * `enum { NAME, ... };` - mugshot colours (include/battle_transition.h),
                            difficulty levels (include/constants/difficulty.h)
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from functools import cached_property

from .repo import Repo

# Constants that exist in the headers but are not selectable values: counts,
# bounds, sentinels and padding. Matching these by suffix keeps the dropdowns
# honest without maintaining a blocklist of specific names.
_NON_VALUE_SUFFIXES = ("_COUNT", "_START", "_END", "_MAX", "_MIN", "_TAG")


@dataclass
class Symbol:
    """One selectable constant, e.g. SPECIES_CHARMANDER."""

    name: str
    value: int | None
    label: str
    raw: str
    is_alias: bool = False


@dataclass
class SymbolTable:
    """An ordered, searchable set of constants of one kind."""

    kind: str
    prefix: str
    source: str
    symbols: list[Symbol] = field(default_factory=list)

    @cached_property
    def by_name(self) -> dict[str, Symbol]:
        return {s.name: s for s in self.symbols}

    def __contains__(self, name: str) -> bool:
        return name in self.by_name

    def __len__(self) -> int:
        return len(self.symbols)

    def get(self, name: str) -> Symbol | None:
        return self.by_name.get(name)

    def canonical(self) -> list[Symbol]:
        """Selectable symbols only - aliases hidden so dropdowns show one entry."""
        return [s for s in self.symbols if not s.is_alias]

    def suggest(self, name: str, limit: int = 5) -> list[str]:
        """Near-miss names, used to make validation errors actionable."""
        import difflib

        return difflib.get_close_matches(name, list(self.by_name), n=limit, cutoff=0.6)


def humanize(name: str, prefix: str) -> str:
    """SPECIES_MR_MIME -> 'Mr Mime'. Display-only; the constant stays the key."""
    body = name[len(prefix):] if name.startswith(prefix) else name
    if not body:
        return name
    return " ".join(part.capitalize() for part in body.split("_") if part)


_DEFINE_RE = re.compile(
    r"^[ \t]*#define[ \t]+(?P<name>\w+)[ \t]+(?P<body>[^\r\n]*?)[ \t]*(?://.*)?$",
    re.MULTILINE,
)


def _strip_block_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/", " ", text, flags=re.DOTALL)


def _eval_int(body: str, known: dict[str, int]) -> int | None:
    """Evaluate a simple constant expression such as `5`, `(1 << 3)` or `(X + 1)`.

    Only the arithmetic actually used by these headers is supported. Anything
    else yields None, which simply means the symbol has no numeric value here -
    it is still listed and still usable.
    """
    expr = body.strip()
    if not expr:
        return None
    resolved = re.sub(r"\b([A-Za-z_]\w*)\b", lambda m: str(known.get(m.group(1), m.group(1))), expr)
    if not re.fullmatch(r"[0-9xXa-fA-F\s()<>+\-*/|&~]+", resolved):
        return None
    try:
        return int(eval(resolved, {"__builtins__": {}}, {}))  # noqa: S307 - arithmetic only, regex-gated
    except Exception:
        return None


def load_defines(repo: Repo, rel: str, prefix: str, kind: str) -> SymbolTable:
    """Collect every `#define <prefix>...` from one header, in file order."""
    table = SymbolTable(kind=kind, prefix=prefix, source=rel)
    try:
        text = _strip_block_comments(repo.read(rel))
    except OSError:
        return table

    known: dict[str, int] = {}
    for match in _DEFINE_RE.finditer(text):
        name, body = match.group("name"), match.group("body").strip()
        if not name.startswith(prefix) or "(" in name:
            continue
        if any(name.endswith(suffix) for suffix in _NON_VALUE_SUFFIXES):
            continue
        value = _eval_int(body, known)
        if value is not None:
            known[name] = value
        # `#define MOVE_DOUBLESLAP MOVE_DOUBLE_SLAP` is a back-compat alias.
        is_alias = bool(re.fullmatch(r"\w+", body)) and body.startswith(prefix)
        table.symbols.append(
            Symbol(name=name, value=value, label=humanize(name, prefix), raw=body, is_alias=is_alias)
        )
    return table


def load_enum(repo: Repo, rel: str, enum_body_prefix: str, kind: str, enum_name: str = "") -> SymbolTable:
    """Collect enumerators sharing a prefix from a C enum block."""
    table = SymbolTable(kind=kind, prefix=enum_body_prefix, source=rel)
    try:
        text = _strip_block_comments(repo.read(rel))
    except OSError:
        return table

    pattern = rf"enum\s+{enum_name}\w*\s*\{{(.*?)\}}" if enum_name else r"enum\s*\w*\s*\{(.*?)\}"
    counter = 0
    for block in re.finditer(pattern, text, re.DOTALL):
        body = block.group(1)
        if enum_body_prefix not in body:
            continue
        counter = 0
        for entry in body.split(","):
            entry = re.sub(r"//.*", "", entry).strip()
            if not entry:
                continue
            if "=" in entry:
                name, _, rhs = entry.partition("=")
                name = name.strip()
                explicit = _eval_int(rhs, {})
                if explicit is not None:
                    counter = explicit
            else:
                name = entry
            if not re.fullmatch(r"\w+", name):
                continue
            if name.startswith(enum_body_prefix) and not any(
                name.endswith(suffix) for suffix in _NON_VALUE_SUFFIXES
            ):
                table.symbols.append(
                    Symbol(name=name, value=counter, label=humanize(name, enum_body_prefix), raw=str(counter))
                )
            counter += 1
        if table.symbols:
            break
    return table


class RepoConstants:
    """Every symbol table the trainer editor needs, read from this repository."""

    def __init__(self, repo: Repo):
        self.repo = repo

        self.species = load_defines(repo, "include/constants/species.h", "SPECIES_", "species")
        self.moves = load_defines(repo, "include/constants/moves.h", "MOVE_", "move")
        self.items = load_defines(repo, "include/constants/items.h", "ITEM_", "item")
        self.abilities = load_defines(repo, "include/constants/abilities.h", "ABILITY_", "ability")
        self.natures = load_defines(repo, "include/constants/pokemon.h", "NATURE_", "nature")
        self.types = load_defines(repo, "include/constants/pokemon.h", "TYPE_", "type")
        self.trainer_ids = load_defines(repo, "include/constants/opponents.h", "TRAINER_", "trainer_id")
        self.ai_flags = load_defines(repo, "include/constants/battle_ai.h", "AI_FLAG_", "ai_flag")
        self.mugshot_colors = load_enum(repo, "include/battle_transition.h", "MUGSHOT_COLOR_", "mugshot")
        self.difficulties = load_enum(
            repo, "include/constants/difficulty.h", "DIFFICULTY_", "difficulty", enum_name="DifficultyLevel"
        )

        trainers_header = "include/constants/trainers.h"
        self.trainer_classes = load_defines(repo, trainers_header, "TRAINER_CLASS_", "trainer_class")
        self.trainer_pics = load_defines(repo, trainers_header, "TRAINER_PIC_", "trainer_pic")
        self.encounter_music = load_defines(
            repo, trainers_header, "TRAINER_ENCOUNTER_MUSIC_", "encounter_music"
        )

        self.limits = Limits(repo)

    @property
    def balls(self) -> list[Symbol]:
        """Poke Balls are ordinary items; the `ball` field only accepts these."""
        return [s for s in self.items.canonical() if s.name.endswith("_BALL")]

    def table(self, kind: str) -> SymbolTable | None:
        return {
            "species": self.species,
            "move": self.moves,
            "item": self.items,
            "ability": self.abilities,
            "nature": self.natures,
            "type": self.types,
            "trainer_id": self.trainer_ids,
            "ai_flag": self.ai_flags,
            "mugshot": self.mugshot_colors,
            "difficulty": self.difficulties,
            "trainer_class": self.trainer_classes,
            "trainer_pic": self.trainer_pics,
            "encounter_music": self.encounter_music,
        }.get(kind)

    def summary(self) -> dict[str, int]:
        return {
            "species": len(self.species.canonical()),
            "moves": len(self.moves.canonical()),
            "items": len(self.items.canonical()),
            "abilities": len(self.abilities.canonical()),
            "natures": len(self.natures.canonical()),
            "types": len(self.types.canonical()),
            "trainer_classes": len(self.trainer_classes.canonical()),
            "trainer_pics": len(self.trainer_pics.canonical()),
            "encounter_music": len(self.encounter_music.canonical()),
            "ai_flags": len(self.ai_flags.canonical()),
            "mugshot_colors": len(self.mugshot_colors.canonical()),
            "trainer_ids": len(self.trainer_ids.canonical()),
        }


class Limits:
    """Numeric bounds read from the repository rather than assumed.

    This matters here: the repository raises MAX_LEVEL to 200, so a validator
    that assumed the usual 100 would reject 31 party members that build fine.
    """

    def __init__(self, repo: Repo):
        from .repo import scrape_define

        # Some limits are written in terms of other constants, e.g.
        # `MAX_PER_STAT_EVS ((P_EV_CAP >= GEN_6) ? 252 : 255)`. Resolve the
        # generation constants and config flags first so those evaluate.
        known: dict[str, int] = {}
        for rel, prefix in (
            ("include/config/general.h", "GEN_"),
            ("include/config/pokemon.h", "P_"),
        ):
            for symbol in load_defines(repo, rel, prefix, "config").symbols:
                if symbol.value is not None:
                    known[symbol.name] = symbol.value

        def number(rel: str, name: str, fallback: int) -> int:
            raw = scrape_define(repo, rel, name)
            if raw is None:
                return fallback
            value = _eval_int(raw, known)
            return fallback if value is None else value

        pokemon_h = "include/constants/pokemon.h"
        global_h = "include/constants/global.h"
        self.max_level = number(pokemon_h, "MAX_LEVEL", 100)
        self.min_level = 1
        self.max_iv = number(pokemon_h, "MAX_PER_STAT_IVS", 31)
        self.max_ev_per_stat = number(pokemon_h, "MAX_PER_STAT_EVS", 252)
        self.max_total_evs = number(pokemon_h, "MAX_TOTAL_EVS", 510)
        self.max_dynamax_level = number(pokemon_h, "MAX_DYNAMAX_LEVEL", 10)
        self.max_friendship = number(pokemon_h, "MAX_FRIENDSHIP", 255)
        self.max_party_size = number(global_h, "PARTY_SIZE", 6)
        self.trainer_name_length = number(global_h, "TRAINER_NAME_LENGTH", 10)
        self.max_mon_moves = number(global_h, "MAX_MON_MOVES", 4)
        self.max_trainer_items = number("include/data.h", "MAX_TRAINER_ITEMS", 4)
        self.trainers_count = number("include/constants/opponents.h", "TRAINERS_COUNT", 0)
        self.max_trainers_count = number("include/constants/opponents.h", "MAX_TRAINERS_COUNT", 0)

    def as_dict(self) -> dict[str, int]:
        return {k: v for k, v in self.__dict__.items() if isinstance(v, int)}
