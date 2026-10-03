"""Turns the parsed model into plain data for a UI, and back again.

The GUI never sees C source or byte offsets; it only sees dictionaries produced
here. That keeps the parsing/writing layer reusable for a future CLI, a test
harness, or another editor entirely.
"""

from __future__ import annotations

from typing import Any

from ..constants import RepoConstants, humanize
from ..repo import Repo
from .model import MON_FIELDS, TRAINER_FIELDS, Field, MonEntry, TrainerEntry
from .parser import TrainerFile, parse_trainers
from .validator import Validator

MUSIC_PREFIX = "TRAINER_ENCOUNTER_MUSIC_"
FEMALE_FLAG = "F_TRAINER_FEMALE"


def split_music_gender(value: str | None) -> tuple[str, str]:
    """`F_TRAINER_FEMALE | TRAINER_ENCOUNTER_MUSIC_COOL` -> ('..._COOL', 'female').

    The struct packs the trainer's gender into the top bit of the encounter
    music byte, so the UI presents them as two controls over one field.
    """
    parts = [p.strip() for p in (value or "").split("|") if p.strip()]
    female = FEMALE_FLAG in parts
    music = next((p for p in parts if p.startswith(MUSIC_PREFIX)), "")
    return music, "female" if female else "male"


def join_music_gender(music: str, gender: str) -> str:
    music = music or f"{MUSIC_PREFIX}MALE"
    return f"{FEMALE_FLAG} | {music}" if gender == "female" else music


def mon_to_dict(mon, index: int = -1) -> dict[str, Any]:
    """One party member as the UI sees it, with absent fields left as None."""
    data: dict[str, Any] = {
        # Sent back on save so an untouched mon can keep its original bytes
        # even if other members of the party moved around it.
        "_sourceIndex": index,
        "species": mon.value("species"),
        "level": mon.value("lvl"),
        "gender": mon.value("gender", "TRAINER_MON_RANDOM_GENDER"),
        "nickname": mon.value("nickname"),
        "heldItem": mon.value("heldItem"),
        "ability": mon.value("ability"),
        "nature": mon.value("nature"),
        "ball": mon.value("ball"),
        "friendship": mon.value("friendship"),
        "isShiny": bool(mon.value("isShiny", False)),
        "teraType": mon.value("teraType"),
        "gigantamaxFactor": bool(mon.value("gigantamaxFactor", False)),
        "shouldUseDynamax": bool(mon.value("shouldUseDynamax", False)),
        "dynamaxLevel": mon.fields["dynamaxLevel"].value_text if mon.has("dynamaxLevel") else None,
        "ivs": mon.value("iv"),
        "evs": mon.value("ev"),
        "moves": list(mon.moves),
        "tags": mon.value("tags") or [],
        "presentFields": sorted(mon.fields.keys()),
    }
    data["speciesLabel"] = humanize(data["species"] or "", "SPECIES_")
    return data


def trainer_to_dict(entry: TrainerEntry, *, full: bool = True) -> dict[str, Any]:
    music, gender = split_music_gender(entry.value("encounterMusic_gender"))
    summary = {
        "key": entry.key,
        "difficulty": entry.difficulty,
        "name": entry.name,
        "trainerClass": entry.trainer_class,
        "classLabel": humanize(entry.trainer_class, "TRAINER_CLASS_"),
        "display": entry.display_name(),
        "partyCount": len(entry.party),
        "partySize": entry.value("partySize"),
        "doubleBattle": bool(entry.value("doubleBattle", False)),
    }
    if not full:
        return summary

    summary.update(
        {
            "trainerPic": entry.value("trainerPic"),
            "music": music,
            "gender": gender,
            "items": entry.value("items") or [],
            "aiFlags": entry.value("aiFlags") or [],
            "mugshotColor": entry.value("mugshotColor"),
            "startingStatus": entry.value("startingStatus"),
            "poolSize": entry.value("poolSize"),
            "presentFields": sorted(entry.fields.keys()),
            "party": [mon_to_dict(mon, i) for i, mon in enumerate(entry.party)],
            "sourceLine": 0,
        }
    )
    return summary


# --------------------------------------------------------------------------
# Applying UI edits back onto the model
# --------------------------------------------------------------------------

def _set(container, name: str, value: Any, specs: dict) -> None:
    """Set a field, creating it if the entry did not have one."""
    spec = specs[name]
    existing = container.fields.get(name)
    if existing is not None:
        existing.set(value, spec)
        return
    container.fields[name] = Field(
        name=name, raw="", value_text=spec.codec.encode(value), value=value, dirty=True
    )
    container.dirty = True
    _reorder(container, specs)


def _drop(container, name: str) -> None:
    """Remove a field entirely, so the emitted C stays as sparse as the original."""
    if name in container.fields:
        del container.fields[name]
        container.dirty = True


def _reorder(container, specs: dict) -> None:
    """Keep fields in the schema's canonical order once a new one is inserted."""
    container.fields = dict(
        sorted(
            container.fields.items(),
            key=lambda kv: specs[kv[0]].order if kv[0] in specs else 9999,
        )
    )


def _set_or_drop(container, name: str, value: Any, specs: dict, *, keep: bool = False) -> None:
    """Set when there is a value, otherwise remove the field.

    `keep` is for fields every entry in this file carries (species, level,
    nature, IVs, gender, dynamaxLevel) - those stay even at default values so
    edited entries keep looking like their neighbours.
    """
    empty = value is None or value == "" or value == [] or value is False
    if empty and not keep:
        _drop(container, name)
    else:
        _set(container, name, value, specs)


def apply_mon(mon: MonEntry, data: dict[str, Any]) -> None:
    spec = MON_FIELDS
    _set_or_drop(mon, "species", data.get("species"), spec, keep=True)
    _set_or_drop(mon, "gender", data.get("gender") or "TRAINER_MON_RANDOM_GENDER", spec, keep=True)
    _set_or_drop(mon, "lvl", data.get("level"), spec, keep=True)
    _set_or_drop(mon, "nature", data.get("nature") or "NATURE_HARDY", spec, keep=True)
    _set_or_drop(mon, "iv", data.get("ivs"), spec, keep=True)

    _set_or_drop(mon, "nickname", data.get("nickname"), spec)
    _set_or_drop(mon, "heldItem", data.get("heldItem"), spec)
    _set_or_drop(mon, "ability", data.get("ability"), spec)
    _set_or_drop(mon, "isShiny", bool(data.get("isShiny")), spec)
    _set_or_drop(mon, "ball", data.get("ball"), spec)
    _set_or_drop(mon, "friendship", data.get("friendship"), spec)
    _set_or_drop(mon, "teraType", data.get("teraType"), spec)
    _set_or_drop(mon, "gigantamaxFactor", bool(data.get("gigantamaxFactor")), spec)
    _set_or_drop(mon, "shouldUseDynamax", bool(data.get("shouldUseDynamax")), spec)

    # EVs: keep an existing all-zero block rather than deleting it, but do not
    # add one to a mon that never had EVs just because the UI showed zeroes.
    evs = data.get("evs")
    if evs and (any(evs) or mon.has("ev")):
        _set(mon, "ev", evs, spec)
    else:
        _drop(mon, "ev")

    moves = [m for m in (data.get("moves") or []) if m]
    if moves != mon.moves:
        mon.moves = moves
        mon.dirty = True
    if moves:
        if not mon.has("moves"):
            mon.fields["moves"] = Field(name="moves", raw="", value_text="", value=moves, dirty=True)
            _reorder(mon, spec)
    else:
        _drop(mon, "moves")

    # `.dynamaxLevel = MAX_DYNAMAX_LEVEL` is on every mon in this file; leave
    # the symbolic text alone unless the UI sent a different number.
    requested = data.get("dynamaxLevel")
    if requested is not None and mon.has("dynamaxLevel"):
        if str(requested) != mon.fields["dynamaxLevel"].value_text:
            _set(mon, "dynamaxLevel", requested, spec)
    elif not mon.has("dynamaxLevel"):
        mon.fields["dynamaxLevel"] = Field(
            name="dynamaxLevel", raw="", value_text="MAX_DYNAMAX_LEVEL", value=None, dirty=True
        )
        _reorder(mon, spec)


def apply_trainer(entry: TrainerEntry, data: dict[str, Any]) -> None:
    spec = TRAINER_FIELDS
    _set_or_drop(entry, "trainerName", data.get("name"), spec,
                 keep=entry.key != "TRAINER_NONE")
    _set_or_drop(entry, "trainerClass", data.get("trainerClass"), spec, keep=True)
    _set_or_drop(entry, "trainerPic", data.get("trainerPic"), spec, keep=True)
    _set_or_drop(entry, "encounterMusic_gender",
                 join_music_gender(data.get("music"), data.get("gender")), spec, keep=True)
    _set_or_drop(entry, "doubleBattle", bool(data.get("doubleBattle")), spec, keep=True)
    _set_or_drop(entry, "items", data.get("items") or [], spec)
    _set_or_drop(entry, "aiFlags", data.get("aiFlags") or [], spec)
    _set_or_drop(entry, "mugshotColor", data.get("mugshotColor"), spec)
    _set_or_drop(entry, "startingStatus", data.get("startingStatus"), spec)

    incoming = data.get("party") or []
    rebuilt: list[MonEntry] = []
    for index, mon_data in enumerate(incoming):
        # `_sourceIndex` lets an unmoved, unedited mon keep its original bytes.
        source = mon_data.get("_sourceIndex")
        if isinstance(source, int) and 0 <= source < len(entry.party):
            mon = entry.party[source]
            if source != index:
                mon.dirty = True  # reordered, so its position in the array changed
        else:
            mon = MonEntry(dirty=True)
        apply_mon(mon, mon_data)
        rebuilt.append(mon)

    if [id(m) for m in rebuilt] != [id(m) for m in entry.party]:
        entry.dirty = True
    entry.party = rebuilt

    # Auto-correct .partySize to match what is actually defined. Four trainers
    # in this repository disagree; editing one quietly fixes it.
    _set_or_drop(entry, "partySize", len(rebuilt), spec, keep=True)


class TrainerService:
    """Loads the repository once and answers UI queries against it."""

    def __init__(self, repo: Repo):
        self.repo = repo
        self.constants = RepoConstants(repo)
        self.file = TrainerFile.load(repo.trainers_h)
        self.validator = Validator(self.constants)

    def reload(self) -> None:
        """Re-read everything, so new species or trainers appear without restart."""
        self.constants = RepoConstants(self.repo)
        self.file = TrainerFile.load(self.repo.trainers_h)
        self.validator = Validator(self.constants)

    # -- queries ---------------------------------------------------------

    def list_trainers(self) -> list[dict[str, Any]]:
        return [trainer_to_dict(e, full=False) for e in self.file.entries]

    def get(self, key: str, difficulty: str = "DIFFICULTY_NORMAL") -> dict[str, Any] | None:
        entry = self.file.by_key(key, difficulty)
        if entry is None:
            return None
        data = trainer_to_dict(entry)
        data["sourceLine"] = self.file.text.count("\n", 0, entry.span[0]) + 1
        return data

    def source_of(self, key: str, difficulty: str = "DIFFICULTY_NORMAL") -> str:
        entry = self.file.by_key(key, difficulty)
        return entry.raw if entry else ""

    # -- option lists for the UI ----------------------------------------

    def options(self) -> dict[str, Any]:
        """Every dropdown's contents, scraped from the repository this run."""
        constants = self.constants

        def pack(table) -> list[dict[str, Any]]:
            return [{"name": s.name, "label": s.label, "value": s.value} for s in table.canonical()]

        return {
            "species": pack(constants.species),
            "moves": pack(constants.moves),
            "items": pack(constants.items),
            "abilities": pack(constants.abilities),
            "natures": pack(constants.natures),
            "types": pack(constants.types),
            "balls": [{"name": s.name, "label": s.label, "value": s.value} for s in constants.balls],
            "trainerClasses": pack(constants.trainer_classes),
            "trainerPics": pack(constants.trainer_pics),
            "encounterMusic": pack(constants.encounter_music),
            "aiFlags": pack(constants.ai_flags),
            "mugshotColors": pack(constants.mugshot_colors),
            "difficulties": pack(constants.difficulties),
            "monGenders": [
                {"name": "TRAINER_MON_RANDOM_GENDER", "label": "Random"},
                {"name": "TRAINER_MON_MALE", "label": "Male"},
                {"name": "TRAINER_MON_FEMALE", "label": "Female"},
            ],
            "limits": constants.limits.as_dict(),
        }

    def repo_info(self) -> dict[str, Any]:
        from ..repo import detect_party_syntax

        free_ids = self.free_trainer_ids()
        return {
            "root": str(self.repo.root),
            "trainersFile": str(self.repo.trainers_h.relative_to(self.repo.root)),
            "competitivePartySyntax": detect_party_syntax(self.repo),
            "trainerCount": len(self.file.entries),
            "partyTotal": sum(len(e.party) for e in self.file.entries),
            "counts": self.constants.summary(),
            "limits": self.constants.limits.as_dict(),
            "freeTrainerIds": free_ids[:16],
            "freeTrainerIdCount": len(free_ids),
        }

    # -- trainer ID bookkeeping -----------------------------------------

    def used_trainer_ids(self) -> dict[str, int]:
        """Constant name -> numeric ID, for every ID declared in opponents.h."""
        return {
            s.name: s.value
            for s in self.constants.trainer_ids.canonical()
            if s.value is not None
        }

    # -- validation and saving -------------------------------------------

    def validate(self, payload: dict[str, Any]):
        return self.validator.validate(payload, existing_keys=set(self.file.keys()))

    def save(self, payload: dict[str, Any]) -> dict[str, Any]:
        """Validate, apply, re-verify, back up, then write src/data/trainers.h.

        Nothing reaches disk until the rendered file has been re-parsed and
        checked, so a bug in the writer surfaces as a refusal rather than as a
        corrupted 1.1 MB source file.
        """
        from .writer import backup, render_file, write_atomic

        report = self.validate(payload)
        if not report.ok:
            return {"ok": False, "stage": "validation", "report": report.to_dict()}

        key = payload["key"]
        difficulty = payload.get("difficulty", "DIFFICULTY_NORMAL")
        entry = self.file.by_key(key, difficulty)
        if entry is None:
            return {
                "ok": False,
                "stage": "lookup",
                "error": f"{key} is not in {self.repo.trainers_h.name}; cannot save.",
            }

        # Work on a throwaway parse so a failure cannot leave the live model
        # half-edited and out of step with the file on disk.
        scratch = TrainerFile(path=self.file.path, text=self.file.text,
                              entries=parse_trainers(self.file.text))
        target = scratch.by_key(key, difficulty)
        apply_trainer(target, payload)

        rendered = render_file(scratch)

        problem = self._verify_render(rendered, key, difficulty, len(scratch.entries))
        if problem:
            return {"ok": False, "stage": "verify", "error": problem}

        backup_path = backup(self.repo.trainers_h, self.repo.backup_dir)
        write_atomic(self.repo.trainers_h, rendered)
        self.reload()

        return {
            "ok": True,
            "stage": "written",
            "key": key,
            "backup": str(backup_path.relative_to(self.repo.root)),
            "report": report.to_dict(),
            "trainer": self.get(key, difficulty),
        }

    def _verify_render(self, rendered: str, key: str, difficulty: str,
                       expected_count: int) -> str | None:
        """Re-parse the proposed file and confirm nothing was lost."""
        try:
            reparsed = parse_trainers(rendered)
        except Exception as exc:
            return f"The generated file could not be parsed back ({exc}). Nothing was written."

        if len(reparsed) != expected_count:
            return (
                f"The generated file has {len(reparsed)} trainers but should have "
                f"{expected_count}. Nothing was written."
            )

        saved = next((e for e in reparsed if e.key == key and e.difficulty == difficulty), None)
        if saved is None:
            return f"{key} disappeared from the generated file. Nothing was written."

        # Everything except the edited trainer must be byte-identical.
        originals = {(e.difficulty, e.key): e.raw for e in self.file.entries}
        for entry in reparsed:
            ident = (entry.difficulty, entry.key)
            if ident == (difficulty, key):
                continue
            if originals.get(ident) != entry.raw:
                return (
                    f"Writing {key} would also have changed {entry.key}. "
                    f"Nothing was written."
                )
        return None

    def free_trainer_ids(self) -> list[int]:
        """Numeric IDs below MAX_TRAINERS_COUNT that nothing has claimed.

        This repository is nearly full: TRAINERS_COUNT is 861 against a
        MAX_TRAINERS_COUNT of 864, and raising the ceiling means moving flags in
        constants/flags.h because TRAINER_FLAGS_END already butts against
        SYSTEM_FLAGS. The UI shows this so a new trainer is never silently
        assigned an ID with no flag behind it.
        """
        limits = self.constants.limits
        taken = set(self.used_trainer_ids().values())
        return [i for i in range(limits.max_trainers_count) if i not in taken]
