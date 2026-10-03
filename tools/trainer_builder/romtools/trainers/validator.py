"""Checks a trainer before it is allowed to reach src/data/trainers.h.

Every message is written to be read by a person: what is wrong, where, and what
to do about it. Unknown constants come with near-miss suggestions, because the
usual cause is a typo or a renamed species rather than a conceptual mistake.

Errors block saving. Warnings do not - they cover things that compile and run
but are probably not what was intended.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from ..constants import RepoConstants

STAT_LABELS = ("HP", "Attack", "Defense", "Speed", "Sp. Attack", "Sp. Defense")


@dataclass
class Issue:
    severity: str          # "error" | "warning"
    message: str
    where: str = ""        # "trainer" or "party"
    monIndex: int | None = None
    fieldName: str = ""

    def to_dict(self) -> dict[str, Any]:
        return {
            "severity": self.severity,
            "message": self.message,
            "where": self.where,
            "monIndex": self.monIndex,
            "field": self.fieldName,
        }


@dataclass
class Report:
    issues: list[Issue] = field(default_factory=list)

    @property
    def errors(self) -> list[Issue]:
        return [i for i in self.issues if i.severity == "error"]

    @property
    def warnings(self) -> list[Issue]:
        return [i for i in self.issues if i.severity == "warning"]

    @property
    def ok(self) -> bool:
        return not self.errors

    def error(self, message: str, **kw) -> None:
        self.issues.append(Issue("error", message, **kw))

    def warn(self, message: str, **kw) -> None:
        self.issues.append(Issue("warning", message, **kw))

    def to_dict(self) -> dict[str, Any]:
        return {
            "ok": self.ok,
            "errorCount": len(self.errors),
            "warningCount": len(self.warnings),
            "issues": [i.to_dict() for i in self.issues],
        }


class Validator:
    def __init__(self, constants: RepoConstants):
        self.constants = constants
        self.limits = constants.limits

    # -- helpers ---------------------------------------------------------

    def _check_symbol(
        self,
        report: Report,
        value: str | None,
        kind: str,
        human: str,
        *,
        required: bool = False,
        **location,
    ) -> bool:
        table = self.constants.table(kind)
        if not value:
            if required:
                report.error(f"{human} is required but not set.", **location)
                return False
            return True
        if table is None or value in table:
            return True
        suggestions = table.suggest(value)
        hint = f" Did you mean {' or '.join(suggestions[:3])}?" if suggestions else ""
        report.error(
            f"{human} is set to {value}, which does not exist in "
            f"{table.source}.{hint}",
            **location,
        )
        return False

    def _check_range(
        self, report: Report, value, low: int, high: int, human: str, **location
    ) -> None:
        if value is None:
            return
        if not isinstance(value, int):
            report.error(f"{human} must be a number, got {value!r}.", **location)
            return
        if value < low or value > high:
            report.error(
                f"{human} is {value}, which is outside the supported range "
                f"{low}-{high} for this repository.",
                **location,
            )

    # -- trainer ---------------------------------------------------------

    def validate(self, data: dict[str, Any], *, existing_keys: set[str] | None = None) -> Report:
        report = Report()
        self._validate_identity(report, data, existing_keys or set())
        self._validate_trainer_fields(report, data)
        self._validate_party(report, data)
        return report

    def _validate_identity(self, report: Report, data: dict, existing_keys: set[str]) -> None:
        key = data.get("key") or ""
        where = {"where": "trainer", "fieldName": "key"}

        if not key:
            report.error("The trainer has no ID constant (e.g. TRAINER_RICK).", **where)
        elif not key.startswith("TRAINER_"):
            report.error(f"Trainer ID {key} must start with TRAINER_.", **where)
        elif data.get("isNew") and key in existing_keys:
            report.error(
                f"{key} already exists. Trainer IDs must be unique - pick another name.",
                **where,
            )
        elif not data.get("isNew") and key not in self.constants.trainer_ids:
            report.error(
                f"{key} is not declared in {self.constants.trainer_ids.source}.", **where
            )

    def _validate_trainer_fields(self, report: Report, data: dict) -> None:
        limits = self.limits
        at = {"where": "trainer"}

        name = data.get("name") or ""
        if not name and data.get("key") != "TRAINER_NONE":
            report.warn(
                "The trainer has no name, so the game will show a blank name in battle.",
                fieldName="name", **at,
            )
        if len(name) > limits.trainer_name_length:
            report.error(
                f"The name {name!r} is {len(name)} characters. "
                f"TRAINER_NAME_LENGTH is {limits.trainer_name_length}, so it would be cut off.",
                fieldName="name", **at,
            )

        self._check_symbol(report, data.get("trainerClass"), "trainer_class",
                           "Trainer class", required=True, fieldName="trainerClass", **at)
        self._check_symbol(report, data.get("trainerPic"), "trainer_pic",
                           "Trainer sprite", required=True, fieldName="trainerPic", **at)
        self._check_symbol(report, data.get("music"), "encounter_music",
                           "Encounter music", required=True, fieldName="music", **at)
        self._check_symbol(report, data.get("mugshotColor"), "mugshot",
                           "Mugshot colour", fieldName="mugshotColor", **at)

        if data.get("gender") not in ("male", "female"):
            report.error(
                f"Trainer gender must be male or female, got {data.get('gender')!r}.",
                fieldName="gender", **at,
            )

        items = data.get("items") or []
        if len(items) > limits.max_trainer_items:
            report.error(
                f"{len(items)} battle items given, but MAX_TRAINER_ITEMS is "
                f"{limits.max_trainer_items}. Remove {len(items) - limits.max_trainer_items}.",
                fieldName="items", **at,
            )
        for item in items:
            self._check_symbol(report, item, "item", f"Battle item {item}",
                               fieldName="items", **at)

        for flag in data.get("aiFlags") or []:
            self._check_symbol(report, flag, "ai_flag", f"AI flag {flag}",
                               fieldName="aiFlags", **at)

        self._validate_ai_dependencies(report, data)

    def _validate_ai_dependencies(self, report: Report, data: dict) -> None:
        """A few AI flags in battle_ai.h document that they require another flag."""
        flags = set(data.get("aiFlags") or [])
        if not flags:
            return
        requirements = {
            "AI_FLAG_PREDICT_INCOMING_MON": "AI_FLAG_PREDICT_SWITCH",
        }
        for flag, needs in requirements.items():
            if flag in flags and needs not in flags and needs in self.constants.ai_flags:
                report.warn(
                    f"{flag} is set without {needs}; battle_ai.h notes that it "
                    f"requires {needs} to have any effect.",
                    where="trainer", fieldName="aiFlags",
                )
        if data.get("doubleBattle") and len(data.get("party") or []) < 2:
            report.error(
                "This is marked as a double battle but has fewer than 2 Pokemon.",
                where="trainer", fieldName="doubleBattle",
            )

    # -- party -----------------------------------------------------------

    def _validate_party(self, report: Report, data: dict) -> None:
        limits = self.limits
        party = data.get("party") or []

        if not party:
            if data.get("key") != "TRAINER_NONE":
                report.error(
                    "The party is empty. A trainer with no Pokemon cannot be battled.",
                    where="party",
                )
            return

        if len(party) > limits.max_party_size:
            report.error(
                f"The party has {len(party)} Pokemon but PARTY_SIZE is "
                f"{limits.max_party_size}. Remove {len(party) - limits.max_party_size}.",
                where="party",
            )

        for index, mon in enumerate(party):
            self._validate_mon(report, mon, index)

    def _validate_mon(self, report: Report, mon: dict, index: int) -> None:
        limits = self.limits
        at = {"where": "party", "monIndex": index}
        label = f"Pokemon {index + 1}"

        # A bad species does not stop the remaining checks: reporting every
        # problem at once beats making the user fix them one save at a time.
        species = mon.get("species")
        self._check_symbol(report, species, "species", f"{label}: species",
                           required=True, fieldName="species", **at)
        if species == "SPECIES_NONE":
            report.error(
                f"{label} is set to SPECIES_NONE, which is not a real Pokemon.",
                fieldName="species", **at,
            )

        self._check_range(report, mon.get("level"), limits.min_level, limits.max_level,
                          f"{label}: level", fieldName="level", **at)

        self._check_symbol(report, mon.get("heldItem"), "item", f"{label}: held item",
                           fieldName="heldItem", **at)
        self._check_symbol(report, mon.get("ability"), "ability", f"{label}: ability",
                           fieldName="ability", **at)
        self._check_symbol(report, mon.get("nature"), "nature", f"{label}: nature",
                           fieldName="nature", **at)
        self._check_symbol(report, mon.get("teraType"), "type", f"{label}: Tera type",
                           fieldName="teraType", **at)

        ball = mon.get("ball")
        if ball and self._check_symbol(report, ball, "item", f"{label}: Ball",
                                       fieldName="ball", **at):
            if not ball.endswith("_BALL"):
                report.error(
                    f"{label}: {ball} is an item but not a Poke Ball, so it cannot "
                    f"be used as the Ball this Pokemon was caught in.",
                    fieldName="ball", **at,
                )

        gender = mon.get("gender")
        valid_genders = ("TRAINER_MON_RANDOM_GENDER", "TRAINER_MON_MALE", "TRAINER_MON_FEMALE")
        if gender and gender not in valid_genders:
            report.error(
                f"{label}: gender is {gender}, expected one of {', '.join(valid_genders)}.",
                fieldName="gender", **at,
            )

        nickname = mon.get("nickname") or ""
        if len(nickname) > limits.trainer_name_length:
            report.warn(
                f"{label}: the nickname {nickname!r} is longer than "
                f"{limits.trainer_name_length} characters and will be shortened in game.",
                fieldName="nickname", **at,
            )

        self._check_range(report, mon.get("friendship"), 0, limits.max_friendship,
                          f"{label}: friendship", fieldName="friendship", **at)

        self._validate_stats(report, mon.get("ivs"), "IV", 0, limits.max_iv, label, at)
        self._validate_stats(report, mon.get("evs"), "EV", 0, limits.max_ev_per_stat, label, at)

        evs = mon.get("evs")
        if evs and sum(evs) > limits.max_total_evs:
            # trainerproc explicitly documents that trainer EVs are not capped
            # at the usual total, so this is informational only.
            report.warn(
                f"{label}: EVs total {sum(evs)}, above the usual cap of "
                f"{limits.max_total_evs}. Trainer parties are not capped, so this "
                f"works, but a player's Pokemon could not reach it.",
                fieldName="evs", **at,
            )

        self._validate_moves(report, mon, label, at)

    def _validate_stats(self, report: Report, values, kind: str, low: int, high: int,
                        label: str, at: dict) -> None:
        if values is None:
            return
        if len(values) != 6:
            report.error(
                f"{label}: {kind}s need 6 values, got {len(values)}.",
                fieldName=kind.lower() + "s", **at,
            )
            return
        for position, value in enumerate(values):
            if not isinstance(value, int) or value < low or value > high:
                report.error(
                    f"{label}: {STAT_LABELS[position]} {kind} is {value}, "
                    f"which is outside {low}-{high}.",
                    fieldName=kind.lower() + "s", **at,
                )

    def _validate_moves(self, report: Report, mon: dict, label: str, at: dict) -> None:
        moves = [m for m in (mon.get("moves") or []) if m]
        if len(moves) > self.limits.max_mon_moves:
            report.error(
                f"{label}: {len(moves)} moves listed but MAX_MON_MOVES is "
                f"{self.limits.max_mon_moves}.",
                fieldName="moves", **at,
            )
        for move in moves:
            self._check_symbol(report, move, "move", f"{label}: move {move}",
                               fieldName="moves", **at)
        if "MOVE_NONE" in moves:
            report.error(
                f"{label}: MOVE_NONE is in the move list. Leave the slot empty instead.",
                fieldName="moves", **at,
            )
        duplicates = {m for m in moves if moves.count(m) > 1}
        for move in sorted(duplicates):
            report.error(
                f"{label}: {move} is listed more than once. A Pokemon cannot know "
                f"the same move twice.",
                fieldName="moves", **at,
            )
