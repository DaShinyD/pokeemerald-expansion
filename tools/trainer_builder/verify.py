#!/usr/bin/env python3
"""Strict verification that the parser understands src/data/trainers.h.

Three checks, all of which must pass before the editor is trusted to write:

  1. Round-trip - re-emitting every trainer from the model must reproduce the
     file byte for byte. Any difference means the parser lost information.
  2. Coverage   - every trainer ID defined in constants/opponents.h must be
     accounted for, and every field/value seen must be a known constant.
  3. Edit       - editing one trainer must change only that trainer's bytes.

Run:  python3 tools/trainer_builder/verify.py
"""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from romtools.constants import RepoConstants  # noqa: E402
from romtools.repo import detect_party_syntax, find_repo  # noqa: E402
from romtools.trainers.parser import TrainerFile  # noqa: E402
from romtools.trainers.writer import emit_trainer, render_file  # noqa: E402

GREEN, RED, YELLOW, DIM, RESET = "\033[32m", "\033[31m", "\033[33m", "\033[2m", "\033[0m"
failures: list[str] = []


def check(ok: bool, label: str, detail: str = "") -> bool:
    mark = f"{GREEN}PASS{RESET}" if ok else f"{RED}FAIL{RESET}"
    print(f"  [{mark}] {label}" + (f"  {DIM}{detail}{RESET}" if detail else ""))
    if not ok:
        failures.append(label)
    return ok


def first_difference(a: str, b: str) -> str:
    for index, (left, right) in enumerate(zip(a, b)):
        if left != right:
            line = a.count("\n", 0, index) + 1
            return f"line {line}: expected {a[index:index + 60]!r} got {b[index:index + 60]!r}"
    return f"length differs: {len(a)} vs {len(b)}"


def verify_editing(repo, constants) -> None:
    """The critical safety property: editing nothing must change nothing.

    The UI loads a trainer into plain dicts and posts them back. If that round
    trip is not an identity, every save would quietly reformat whatever the
    parser normalised, so this is checked across all 860 trainers at once.
    """
    from romtools.trainers.parser import parse_trainers
    from romtools.trainers.service import apply_trainer, trainer_to_dict
    from romtools.trainers.validator import Validator
    from romtools.trainers.writer import render_file

    trainer_file = TrainerFile.load(repo.trainers_h)
    originals = {e.key: e.raw for e in trainer_file.entries}

    # The four trainers with a broken .partySize are expected to change, because
    # saving deliberately corrects it.
    expected_fixes = {
        e.key for e in trainer_file.entries
        if isinstance(e.value("partySize"), int) and e.value("partySize") != len(e.party)
    }

    for entry in trainer_file.entries:
        apply_trainer(entry, trainer_to_dict(entry))

    changed = {e.key for e in trainer_file.entries if emit_trainer(e) != originals[e.key]}
    spurious = sorted(changed - expected_fixes)
    check(not spurious, "loading and re-saving all 860 trainers unchanged is a no-op",
          f"{len(spurious)} drifted: {spurious[:5]}" if spurious else "")
    check(changed == expected_fixes,
          f"only the {len(expected_fixes)} broken .partySize entries get corrected",
          f"corrected {sorted(changed)}")

    # A real edit: swap two party members and bump a level.
    fresh = TrainerFile.load(repo.trainers_h)
    target = next(e for e in fresh.entries if len(e.party) >= 3 and e.key not in expected_fixes)
    data = trainer_to_dict(target)
    before = [m["species"] for m in data["party"]]
    data["party"][0], data["party"][1] = data["party"][1], data["party"][0]
    data["party"][0]["level"] = 61
    apply_trainer(target, data)

    rendered = render_file(fresh)
    reparsed = parse_trainers(rendered)
    saved = next(e for e in reparsed if e.key == target.key)
    after = [m.value("species") for m in saved.party]

    check(after == [before[1], before[0], *before[2:]],
          f"reordering {target.key}'s party survives a write/read cycle",
          f"{after[:3]} vs expected {[before[1], before[0], before[2]]}")
    check(saved.party[0].value("lvl") == 61, "the level change survives too")
    check(saved.value("partySize") == len(saved.party), ".partySize stays consistent")
    check(len(reparsed) == len(fresh.entries), "no trainers were lost or duplicated")

    untouched = [e for e in reparsed if e.key != target.key]
    drifted = [e.key for e in untouched if originals.get(e.key) != e.raw]
    check(not drifted, f"the other {len(untouched)} trainers are byte-identical",
          f"{drifted[:5]}" if drifted else "")

    # Validation must reject nonsense rather than emitting it.
    validator = Validator(constants)
    bad = trainer_to_dict(next(e for e in fresh.entries if e.party))
    bad["party"][0]["species"] = "SPECIES_CHARMANDR"
    bad["party"][0]["level"] = 999
    bad["party"][0]["moves"] = ["MOVE_TACKLE", "MOVE_TACKLE"]
    report = validator.validate(bad)
    check(not report.ok, "validator rejects a typo'd species, bad level and duplicate move",
          f"{len(report.errors)} errors")
    messages = " ".join(i.message for i in report.errors)
    check("SPECIES_CHARMANDER" in messages, "and suggests the correct species name")
    check(f"1-{constants.limits.max_level}" in messages,
          f"and uses this repo's MAX_LEVEL of {constants.limits.max_level}")

    good = trainer_to_dict(next(e for e in fresh.entries if len(e.party) >= 2))
    check(validator.validate(good).ok, "validator accepts an unmodified real trainer")


def main() -> int:
    repo = find_repo(Path(__file__).resolve())
    print(f"\n{YELLOW}Repository{RESET}  {repo.root}")

    uses_party = detect_party_syntax(repo)
    print(
        f"{YELLOW}Source{RESET}      "
        + (
            "src/data/trainers.party (COMPETITIVE_PARTY_SYNTAX is on)"
            if uses_party
            else "src/data/trainers.h (COMPETITIVE_PARTY_SYNTAX is off)"
        )
    )

    print(f"\n{YELLOW}Constants scraped from the repository{RESET}")
    constants = RepoConstants(repo)
    for name, count in constants.summary().items():
        print(f"  {count:>5}  {name}")
    print(f"\n{YELLOW}Limits read from the repository{RESET}")
    for name, value in constants.limits.as_dict().items():
        print(f"  {value:>5}  {name}")

    print(f"\n{YELLOW}1. Parsing{RESET}")
    trainer_file = TrainerFile.load(repo.trainers_h)
    party_total = sum(len(e.party) for e in trainer_file.entries)
    check(True, f"parsed {len(trainer_file.entries)} trainers, {party_total} party members")

    declared = {s.name for s in constants.trainer_ids.canonical()}
    parsed = {e.key for e in trainer_file.entries}
    unknown = sorted(parsed - declared)
    check(not unknown, "every trainer in trainers.h has an ID in opponents.h",
          f"unknown: {unknown[:5]}" if unknown else "")

    duplicates = sorted({k for k in parsed if [e.key for e in trainer_file.entries].count(k) > 1})
    check(not duplicates, "no duplicate trainer entries", f"{duplicates[:5]}" if duplicates else "")

    print(f"\n{YELLOW}2. Round-trip (byte-exact){RESET}")
    mismatched = [e.key for e in trainer_file.entries if emit_trainer(e) != e.raw]
    check(not mismatched, f"all {len(trainer_file.entries)} trainers re-emit identically",
          f"{len(mismatched)} differ: {mismatched[:5]}" if mismatched else "")

    rendered = render_file(trainer_file)
    same = rendered == trainer_file.text
    check(same, "whole file re-renders byte-for-byte",
          "" if same else first_difference(trainer_file.text, rendered))

    print(f"\n{YELLOW}3. Every value resolves to a real constant{RESET}")
    checks = [
        ("species", "species", lambda m: [m.value("species")]),
        ("move", "moves", lambda m: m.moves),
        ("item", "heldItem", lambda m: [m.value("heldItem")]),
        ("ability", "ability", lambda m: [m.value("ability")]),
        ("nature", "nature", lambda m: [m.value("nature")]),
    ]
    for kind, label, extract in checks:
        table = constants.table(kind)
        bad: list[str] = []
        for entry in trainer_file.entries:
            for mon in entry.party:
                for name in extract(mon):
                    if name and name not in table:
                        bad.append(f"{entry.key}:{name}")
        check(not bad, f"all .{label} values exist in {Path(table.source).name}",
              f"{len(bad)} unknown: {sorted(set(bad))[:5]}" if bad else "")

    for kind, field_name in (
        ("trainer_class", "trainerClass"),
        ("trainer_pic", "trainerPic"),
        ("mugshot", "mugshotColor"),
    ):
        table = constants.table(kind)
        bad = [
            f"{e.key}:{e.value(field_name)}"
            for e in trainer_file.entries
            if e.value(field_name) and e.value(field_name) not in table
        ]
        check(not bad, f"all .{field_name} values exist", f"{sorted(set(bad))[:5]}" if bad else "")

    ai_table = constants.ai_flags
    bad_ai = [
        f"{e.key}:{flag}"
        for e in trainer_file.entries
        for flag in (e.value("aiFlags") or [])
        if flag not in ai_table
    ]
    check(not bad_ai, "all .aiFlags values exist in battle_ai.h", f"{sorted(set(bad_ai))[:5]}" if bad_ai else "")

    # Not a parser check: .partySize is authored by hand and can disagree with
    # the party it labels. Where it is too low the extra mons never appear in
    # battle, so these are pre-existing repository bugs worth surfacing.
    bad_size = [
        (e.key, e.value("partySize"), len(e.party))
        for e in trainer_file.entries
        if isinstance(e.value("partySize"), int) and e.value("partySize") != len(e.party)
    ]
    if bad_size:
        print(f"  [{YELLOW}WARN{RESET}] {len(bad_size)} trainers have .partySize "
              f"disagreeing with their party:")
        for key, declared_size, actual in bad_size:
            lost = actual - declared_size
            note = f"{lost} mon(s) unreachable in battle" if lost > 0 else "reads past the party"
            print(f"         {DIM}{key}: partySize={declared_size}, {actual} defined - {note}{RESET}")
    else:
        check(True, ".partySize matches the actual party length everywhere")

    print(f"\n{YELLOW}4. Editing is surgical{RESET}")
    probe = next((e for e in trainer_file.entries if e.party and e.value("trainerName")), None)
    if probe is None:
        check(False, "found a trainer to test-edit")
    else:
        from romtools.trainers.model import MON_FIELDS

        original_rendered = render_file(trainer_file)
        probe.party[0].fields["lvl"].set(77, MON_FIELDS["lvl"])
        edited = render_file(trainer_file)

        delta = len(
            [
                1
                for a, b in zip(original_rendered.splitlines(), edited.splitlines())
                if a != b
            ]
        )
        check(delta == 1, f"editing {probe.key}'s first mon level changed exactly 1 line",
              f"changed {delta} lines")
        check(".lvl = 77," in edited, "the edit is present in the output")

        untouched = [e for e in trainer_file.entries if e is not probe]
        intact = all(emit_trainer(e) == e.raw for e in untouched)
        check(intact, f"the other {len(untouched)} trainers are still byte-identical")

        probe.party[0].fields["lvl"].set(probe.party[0].fields["lvl"].value, MON_FIELDS["lvl"])

    print(f"\n{YELLOW}5. Edit pipeline (in memory, nothing written){RESET}")
    verify_editing(repo, constants)

    print()
    if failures:
        print(f"{RED}{len(failures)} check(s) failed:{RESET}")
        for name in failures:
            print(f"  - {name}")
        return 1
    print(f"{GREEN}All checks passed.{RESET} The parser round-trips this repository exactly.\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
