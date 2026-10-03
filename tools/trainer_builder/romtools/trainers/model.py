"""The internal trainer model, and the field specs that describe this repository.

Design note - why fields keep their raw source text
---------------------------------------------------
src/data/trainers.h is a 1.1 MB hand-maintained file. Reformatting it wholesale
would produce an unreviewable diff and risk losing detail the parser did not
model. So every field, every party member and every trainer keeps the exact
source text it was parsed from. On save we re-emit the original bytes for
anything untouched and generate fresh C only for what actually changed.

That gives two useful guarantees:
  * a load/save cycle with no edits is a no-op at the byte level, and
  * editing one trainer leaves the other 859 completely untouched.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field as dc_field
from typing import Any, Callable

TRAINER_INDENT = " " * 8
MON_INDENT = " " * 12
MOVE_INDENT = " " * 16


# --------------------------------------------------------------------------
# Value codecs: C source text <-> python value
# --------------------------------------------------------------------------

@dataclass(frozen=True)
class Codec:
    decode: Callable[[str], Any]
    encode: Callable[[Any], str]


def _dec_ident(raw: str) -> str:
    return raw.strip()


def _enc_ident(value: Any) -> str:
    return str(value).strip()


def _dec_int(raw: str) -> int | None:
    text = raw.strip()
    try:
        return int(text, 0)
    except ValueError:
        return None  # a symbolic constant such as MAX_DYNAMAX_LEVEL


def _enc_int(value: Any) -> str:
    return str(value)


def _dec_bool(raw: str) -> bool:
    return raw.strip().upper() in ("TRUE", "1")


def _enc_bool(value: Any) -> str:
    return "TRUE" if value else "FALSE"


def _dec_string(raw: str) -> str:
    """`_("SAWYER")` or `COMPOUND_STRING("Kevin")` -> the text inside."""
    match = re.search(r'"((?:[^"\\]|\\.)*)"', raw)
    return match.group(1) if match else ""


def _dec_flags(raw: str) -> list[str]:
    """`A | B | C` -> ['A', 'B', 'C']."""
    body = raw.strip()
    if not body or body == "0":
        return []
    return [part.strip() for part in body.split("|") if part.strip()]


def _enc_flags(value: Any) -> str:
    flags = [f for f in (value or []) if f]
    return " | ".join(flags) if flags else "0"


def _dec_list(raw: str) -> list[str]:
    """`{ ITEM_A, ITEM_B }` -> ['ITEM_A', 'ITEM_B']."""
    body = raw.strip()
    if body.startswith("{"):
        body = body[1:]
    if body.endswith("}"):
        body = body[:-1]
    return [part.strip() for part in body.split(",") if part.strip()]


def _enc_list(value: Any) -> str:
    return "{ " + ", ".join(value) + " }" if value else "{ }"


def _dec_stats(raw: str) -> list[int] | None:
    """`TRAINER_PARTY_IVS(0, 0, 0, 0, 0, 0)` -> [0, 0, 0, 0, 0, 0]."""
    match = re.search(r"\(([^)]*)\)", raw)
    if not match:
        return None
    parts = [p.strip() for p in match.group(1).split(",") if p.strip()]
    try:
        return [int(p, 0) for p in parts]
    except ValueError:
        return None


def _dec_expr(raw: str) -> str:
    """Normalise a `A | B` expression that may be wrapped across lines.

    `.encounterMusic_gender` is written as two lines throughout this file:

        .encounterMusic_gender =
            F_TRAINER_FEMALE | TRAINER_ENCOUNTER_MUSIC_COOL,

    Decoding to a canonical single-line form means a value the UI sends back
    unchanged compares equal, so the field stays clean and keeps its original
    bytes instead of being silently reflowed on every save.
    """
    return " | ".join(part.strip() for part in raw.split("|") if part.strip())


IDENT = Codec(_dec_ident, _enc_ident)
EXPR = Codec(_dec_expr, _enc_ident)
INT = Codec(_dec_int, _enc_int)
BOOL = Codec(_dec_bool, _enc_bool)
FLAGS = Codec(_dec_flags, _enc_flags)
LIST = Codec(_dec_list, _enc_list)
NAME = Codec(_dec_string, lambda v: f'_("{v}")')
NICKNAME = Codec(_dec_string, lambda v: f'COMPOUND_STRING("{v}")')
IVS = Codec(_dec_stats, lambda v: "TRAINER_PARTY_IVS(" + ", ".join(str(x) for x in v) + ")")
EVS = Codec(_dec_stats, lambda v: "TRAINER_PARTY_EVS(" + ", ".join(str(x) for x in v) + ")")


# --------------------------------------------------------------------------
# Field specs - the schema, derived from struct Trainer / struct TrainerMon
# --------------------------------------------------------------------------

@dataclass(frozen=True)
class FieldSpec:
    name: str
    codec: Codec
    kind: str            # drives the UI widget and the validator
    label: str
    required: bool = False
    order: int = 0       # canonical emission order for newly written entries


def _specs(rows: list[FieldSpec]) -> dict[str, FieldSpec]:
    return {spec.name: spec for spec in rows}


# Ordering mirrors how entries are already laid out in src/data/trainers.h.
TRAINER_FIELDS = _specs([
    FieldSpec("trainerName", NAME, "text", "Name", required=True, order=10),
    FieldSpec("trainerClass", IDENT, "trainer_class", "Class", required=True, order=20),
    FieldSpec("trainerPic", IDENT, "trainer_pic", "Sprite / Pic", required=True, order=30),
    FieldSpec("encounterMusic_gender", EXPR, "music_gender", "Music & Gender", required=True, order=40),
    FieldSpec("items", LIST, "item_list", "Battle Items", order=50),
    FieldSpec("doubleBattle", BOOL, "bool", "Double Battle", order=60),
    FieldSpec("aiFlags", FLAGS, "ai_flags", "AI Flags", order=70),
    FieldSpec("startingStatus", IDENT, "starting_status", "Starting Status", order=80),
    FieldSpec("mugshotColor", IDENT, "mugshot", "Mugshot Colour", order=90),
    FieldSpec("partySize", INT, "derived_int", "Party Size", order=100),
    FieldSpec("poolSize", INT, "int", "Pool Size", order=110),
    FieldSpec("poolRuleIndex", IDENT, "ident", "Pool Ruleset", order=120),
    FieldSpec("poolPickIndex", IDENT, "ident", "Pool Pick", order=130),
    FieldSpec("poolPruneIndex", IDENT, "ident", "Pool Prune", order=140),
    FieldSpec("party", IDENT, "party", "Party", required=True, order=150),
])

MON_FIELDS = _specs([
    FieldSpec("species", IDENT, "species", "Species", required=True, order=10),
    FieldSpec("gender", IDENT, "mon_gender", "Gender", order=20),
    FieldSpec("nickname", NICKNAME, "text", "Nickname", order=30),
    FieldSpec("heldItem", IDENT, "item", "Held Item", order=40),
    FieldSpec("ability", IDENT, "ability", "Ability", order=50),
    FieldSpec("isShiny", BOOL, "bool", "Shiny", order=60),
    FieldSpec("iv", IVS, "stats", "IVs", order=70),
    FieldSpec("ev", EVS, "stats", "EVs", order=80),
    FieldSpec("lvl", INT, "level", "Level", required=True, order=90),
    FieldSpec("ball", IDENT, "ball", "Ball", order=100),
    FieldSpec("friendship", INT, "friendship", "Friendship", order=110),
    FieldSpec("nature", IDENT, "nature", "Nature", order=120),
    FieldSpec("dynamaxLevel", INT, "dynamax", "Dynamax Level", order=130),
    FieldSpec("gigantamaxFactor", BOOL, "bool", "Gigantamax", order=140),
    FieldSpec("shouldUseDynamax", BOOL, "bool", "Should Dynamax", order=150),
    FieldSpec("teraType", IDENT, "type", "Tera Type", order=160),
    FieldSpec("tags", FLAGS, "pool_tags", "Pool Tags", order=170),
    FieldSpec("moves", IDENT, "moves", "Moves", order=180),
])


# --------------------------------------------------------------------------
# Parsed entities
# --------------------------------------------------------------------------

@dataclass
class Field:
    """One `.name = value,` in the source, with its original bytes."""

    name: str
    raw: str                 # whole field incl. indentation and trailing newline
    value_text: str          # just the value expression
    value: Any               # decoded value
    dirty: bool = False

    def set(self, value: Any, spec: FieldSpec) -> None:
        if value == self.value:
            return
        self.value = value
        self.value_text = spec.codec.encode(value)
        self.dirty = True


@dataclass
class MonEntry:
    """One `struct TrainerMon` initialiser inside a trainer's party."""

    fields: dict[str, Field] = dc_field(default_factory=dict)
    raw: str = ""
    dirty: bool = False
    moves: list[str] = dc_field(default_factory=list)

    def value(self, name: str, default: Any = None) -> Any:
        f = self.fields.get(name)
        return default if f is None else f.value

    def has(self, name: str) -> bool:
        return name in self.fields

    @property
    def species(self) -> str:
        return self.value("species", "SPECIES_NONE")

    @property
    def level(self) -> Any:
        return self.value("lvl")

    def to_dict(self) -> dict[str, Any]:
        data: dict[str, Any] = {name: f.value for name, f in self.fields.items()}
        data["moves"] = list(self.moves)
        if "moves" in self.fields:
            data.pop("moves", None)
            data["moves"] = list(self.moves)
        return data


@dataclass
class TrainerEntry:
    """One `[DIFFICULTY_X][TRAINER_Y] = { ... },` block."""

    key: str
    difficulty: str
    span: tuple[int, int]            # byte range in the original file
    raw: str                         # exact original text of the whole block
    header: str                      # `    [D][K] =\n    {\n`
    footer: str                      # `    },\n`
    party_prefix: str                # `        .party = (...)\n        {\n`
    party_suffix: str                # `        },\n`
    fields: dict[str, Field] = dc_field(default_factory=dict)
    party: list[MonEntry] = dc_field(default_factory=list)
    dirty: bool = False

    def value(self, name: str, default: Any = None) -> Any:
        f = self.fields.get(name)
        return default if f is None else f.value

    @property
    def name(self) -> str:
        return self.value("trainerName", "") or ""

    @property
    def trainer_class(self) -> str:
        return self.value("trainerClass", "") or ""

    def display_name(self) -> str:
        """'Hiker Sawyer' style label for the trainer list."""
        from ..constants import humanize

        cls = humanize(self.trainer_class, "TRAINER_CLASS_") if self.trainer_class else ""
        return f"{cls} {self.name}".strip() or self.key

    def mark_dirty(self) -> None:
        self.dirty = True
