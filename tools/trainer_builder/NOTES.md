# How trainer data works in *this* repository

Written from inspection of this checkout, not from upstream pokeemerald-expansion
documentation. Where the two disagree, this file describes what is actually here.

## The source of truth is `src/data/trainers.h`, not `trainers.party`

Upstream expansion can author trainers in Showdown-style "competitive syntax" in
`src/data/trainers.party`, which `tools/trainerproc` converts into
`src/data/trainers.h` during the build. **That pipeline is switched off here.**

`include/config/general.h`:

```c
#define COMPETITIVE_PARTY_SYNTAX     FALSE
```

`Makefile` gates the conversion rule on it:

```make
ifeq ($(COMPETITIVE_PARTY_SYNTAX),1)
%.h: %.party ; $(CPP) $(CPPFLAGS) -traditional-cpp - < $< | $(TRAINERPROC) -o $@ -i $< -
endif
```

With the flag false, the rule never exists, nothing regenerates `trainers.h`, and
the file is simply `#include`d into the trainer table in `src/data.c`:

```c
const struct Trainer gTrainers[DIFFICULTY_COUNT][TRAINERS_COUNT] =
{
#include "data/trainers.h"
};
```

Corroborating evidence:

| | `trainers.h` | `trainers.party` |
|---|---|---|
| Last modified | 2026-09-27 | 2026-05-01 |
| Trainers | 860 | 855 |
| Contains `TRAINER_BROCK`, `TRAINER_FALKNER`, `TRAINER_LANCE`, `TRAINER_DASH_*` | yes | no |
| Contains vanilla trainers deleted from the game | no | yes |

**`trainers.party` is stale and unused.** Editing it would change nothing in the
built ROM. This tool reads and writes `src/data/trainers.h`.

## Data shapes

`include/data.h` defines both structs. Fields actually present in `trainers.h`
today, with occurrence counts:

**Trainer level** (860 entries) - `trainerClass`, `trainerPic`,
`encounterMusic_gender`, `doubleBattle`, `partySize`, `party` on all of them;
`trainerName` on 859 (`TRAINER_NONE` has none); `aiFlags` on 846; `items` on 144;
`mugshotColor` on 44.

**Mon level** (2147 members) - `species`, `gender`, `iv`, `lvl`, `nature`,
`dynamaxLevel` on all of them; `moves` on 719; `heldItem` on 524; `ability` on
365; `ev` on 362; `isShiny` on 10; `nickname` on 2.

Supported by the struct but unused in the data: `startingStatus`, `poolSize`,
`poolRuleIndex`, `poolPickIndex`, `poolPruneIndex`, `ball`, `friendship`,
`teraType`, `gigantamaxFactor`, `shouldUseDynamax`, `tags`.

Two details that are easy to get wrong:

* `encounterMusic_gender` packs gender into the top bit, so female trainers read
  `F_TRAINER_FEMALE | TRAINER_ENCOUNTER_MUSIC_COOL`. 287 entries use the flag.
* `TRAINER_PARTY_IVS(hp, atk, def, speed, spatk, spdef)` and
  `TRAINER_PARTY_EVS(...)` take arguments in that order. `TRAINER_PARTY_EVS`
  then *stores* them as `{hp,atk,def,spatk,spdef,speed}`, which is not the
  argument order - only the argument order matters when authoring.

## Trainer IDs are nearly exhausted

IDs are plain `#define`s in `include/constants/opponents.h`, running 0 to 860.

```c
#define TRAINERS_COUNT                      861
#define MAX_TRAINERS_COUNT                  864
```

That leaves **3 usable IDs** (861, 862, 863). Beyond that, `constants/flags.h`
has `TRAINER_FLAGS_END = TRAINER_FLAGS_START + MAX_TRAINERS_COUNT - 1` (0x85F)
sitting directly below `SYSTEM_FLAGS` (0x860), so raising the ceiling means
moving flags. The tool surfaces the remaining count and will not hand out an ID
past `MAX_TRAINERS_COUNT`.

Note that IDs here are heavily repurposed rather than appended - several carry
comments like `// now a gym leader (previously match call thalia)`. New IDs must
come from the free list, never from "highest + 1" reasoning alone.

## Other repository-specific deviations

* **`MAX_LEVEL` is 200**, not 100 (`include/constants/pokemon.h`). 31 party
  members are already above level 100. A validator assuming 100 would wrongly
  reject working data.
* **Difficulty**: `gTrainers` is `[DIFFICULTY_COUNT][TRAINERS_COUNT]` with
  `EASY`/`NORMAL`/`HARD` declared, but only `DIFFICULTY_NORMAL` is populated.
* **Custom content** throughout: species such as `SPECIES_SLIFER` and
  `SPECIES_GLACISTER`, class `TRAINER_CLASS_DUELIST`, pics `TRAINER_PIC_YUGI`
  and `TRAINER_PIC_DASH`. This is why every list is scraped from the headers at
  runtime rather than shipped with the tool.
* **Trainer pools** (`src/trainer_pools.c`, `include/trainer_pools.h`) exist and
  the struct carries the five pool fields, but no trainer uses them.

## Pre-existing data bugs found while verifying

Four trainers have a `.partySize` that disagrees with the party they label:

| Trainer | `.partySize` | Defined | Effect |
|---|---|---|---|
| `TRAINER_JUAN_1` | 5 | 6 | last mon never appears |
| `TRAINER_WALLY_VR_1` | 5 | 6 | last mon never appears |
| `TRAINER_BRENDAN_ROUTE_110_MUDKIP` | 3 | 4 | last mon never appears |
| `TRAINER_MYLES` | 4 | 3 | **reads past the end of the party** |

`TRAINER_MYLES` is the serious one: the game is told there are four Pokemon in an
array holding three. The tool flags all four but changes nothing on its own.

## An unrelated pre-existing oddity

`src/data/trainers.party` and `src/data/battle_partners.party` permanently show
as modified in `git status`. `git ls-files --eol` explains why: both have CRLF in
the working tree against LF in the index, and `.gitattributes` has no `*.party`
rule to normalise them. Their mtimes (January and May) predate this tool, so it
is not something the builder did. Adding `*.party text eol=lf` to
`.gitattributes` and re-checking them out would clear it. Harmless either way,
since neither file is used by the build.

## Build

Built from WSL Ubuntu with `make -j32` (the entire contents of `.bash_history`).
WSL has `make 4.4.1` and `/usr/bin/arm-none-eabi-gcc`. Windows itself has no
Python, Node, make or git on PATH, and a .NET runtime but no SDK.

`romtools/build.py` does not hardcode the command: it reads the last `make ...`
line out of `.bash_history` and falls back to `make -j<cpu count>`. Whatever it
chose is shown in the UI next to the result, so there is never any doubt.

## Tool architecture

```
romtools/repo.py          locate the checkout, read config flags
romtools/constants.py     scrape live symbol tables from the headers
romtools/build.py         run the project's real build, parse diagnostics
romtools/trainers/
    model.py              dataclasses + the field schema
    parser.py             trainers.h -> model, recording byte spans
    validator.py          plain-English checks, run before any write
    writer.py             model -> C, spliced back in place
    service.py            model <-> plain dicts for any front end
server.py                 stdlib HTTP server, JSON API + static files
web/                      the browser UI
verify.py                 round-trip, edit-pipeline and consistency checks
inspect_trainer.py        headless inspection of a trainer
test_save.sh              end-to-end save test against a running server
restart.sh                restart the dev server
```

The parsing/writing layers know nothing about the UI, so a future encounter or
shop editor can reuse `repo.py` and `constants.py` unchanged.

### What happens on save

1. The payload is validated. Errors block the save outright; warnings do not.
2. Edits are applied to a **throwaway re-parse**, so a failure cannot leave the
   live model out of step with the file on disk.
3. The whole file is rendered, then **re-parsed and checked**: the trainer count
   must be unchanged, the edited trainer must still be there, and every other
   trainer must be byte-identical. Any discrepancy aborts before touching disk.
4. A timestamped backup is written to `tools/trainer_builder/backups/`.
5. The file is written atomically (temp file in the same directory, then
   `os.replace`), so an interrupted write cannot truncate the original.

`.partySize` is recalculated from the actual party on every save, which quietly
fixes the four broken entries listed above as you open them.

### Why byte spans

`trainers.h` is 1.1 MB of hand-maintained C. Every field, party member and
trainer keeps the exact source text it was parsed from; re-emission reuses those
bytes for anything untouched and generates fresh C only for what changed. A
load/save cycle with no edits is therefore a byte-level no-op, and editing one
trainer provably leaves the other 859 untouched. `verify.py` asserts both.
