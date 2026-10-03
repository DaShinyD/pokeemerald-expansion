#!/usr/bin/env python3
"""Print a trainer the way the model understands it, without the GUI.

Useful for checking the parser against the raw file, and for diffing a trainer
before and after an edit.

    python3 tools/trainer_builder/inspect_trainer.py TRAINER_JUAN_1
    python3 tools/trainer_builder/inspect_trainer.py --search juan
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from romtools.repo import find_repo  # noqa: E402
from romtools.trainers.service import TrainerService  # noqa: E402

DIM, BOLD, CYAN, YELLOW, RESET = "\033[2m", "\033[1m", "\033[36m", "\033[33m", "\033[0m"

# Argument order of TRAINER_PARTY_IVS/EVS in include/data.h:
#   (hp, atk, def, speed, spatk, spdef)
STATS = ["HP", "Atk", "Def", "Spe", "SpA", "SpD"]


def fmt_stats(values) -> str:
    if not values:
        return ""
    return " / ".join(f"{v} {STATS[i]}" for i, v in enumerate(values))


def show(service: TrainerService, key: str) -> None:
    data = service.get(key)
    if data is None:
        print(f"No trainer named {key}")
        return

    print(f"\n{BOLD}{data['display']}{RESET}  {DIM}{key} @ {service.repo.trainers_h.name}"
          f":{data['sourceLine']}{RESET}")
    print(f"  Class        {data['trainerClass']}")
    print(f"  Pic          {data['trainerPic']}")
    print(f"  Music        {data['music']}  {DIM}({data['gender']}){RESET}")
    print(f"  Double       {'Yes' if data['doubleBattle'] else 'No'}")
    if data["mugshotColor"]:
        print(f"  Mugshot      {data['mugshotColor']}")
    if data["items"]:
        print(f"  Items        {', '.join(data['items'])}")
    print(f"  AI           {' | '.join(data['aiFlags']) if data['aiFlags'] else DIM + 'none' + RESET}")

    size, actual = data["partySize"], data["partyCount"]
    flag = "" if size == actual else f"  {YELLOW}<- declared {size}, defined {actual}{RESET}"
    print(f"\n  {CYAN}Party ({actual}){RESET}{flag}")

    for index, mon in enumerate(data["party"], 1):
        title = mon["speciesLabel"]
        if mon["nickname"]:
            title += f' "{mon["nickname"]}"'
        if mon["isShiny"]:
            title += " *shiny*"
        print(f"\n    [{index}] {BOLD}{title}{RESET}  Lv. {mon['level']}")
        for caption, value in (
            ("Ability", mon["ability"]),
            ("Item", mon["heldItem"]),
            ("Nature", mon["nature"]),
            ("Ball", mon["ball"]),
            ("Friendship", mon["friendship"]),
            ("Tera", mon["teraType"]),
        ):
            if value:
                print(f"        {caption:<11} {value}")
        if mon["gender"] != "TRAINER_MON_RANDOM_GENDER":
            print(f"        {'Gender':<11} {mon['gender']}")
        if mon["ivs"]:
            print(f"        {'IVs':<11} {fmt_stats(mon['ivs'])}")
        if mon["evs"]:
            print(f"        {'EVs':<11} {fmt_stats(mon['evs'])}")
        if mon["moves"]:
            for move in mon["moves"]:
                print(f"        - {move}")
        else:
            print(f"        {DIM}- no moves set (game uses last 4 level-up moves){RESET}")


def main() -> int:
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("keys", nargs="*", help="trainer constants, e.g. TRAINER_JUAN_1")
    cli.add_argument("--search", help="list trainers matching a term")
    args = cli.parse_args()

    service = TrainerService(find_repo(Path(__file__).resolve()))

    if args.search:
        term = args.search.lower()
        for trainer in service.list_trainers():
            haystack = f"{trainer['display']} {trainer['key']}".lower()
            if term in haystack:
                print(f"  {trainer['key']:<40} {trainer['display']}  ({trainer['partyCount']} mons)")
        return 0

    if not args.keys:
        cli.error("give at least one trainer constant, or use --search")
    for key in args.keys:
        show(service, key)
    print()
    return 0


if __name__ == "__main__":
    sys.exit(main())
