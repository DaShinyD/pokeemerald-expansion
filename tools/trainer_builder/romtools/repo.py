"""Locates the pokeemerald-expansion repository and the files this tool cares about.

Everything is derived from the repository at runtime. Nothing about trainer data,
species lists or build commands is baked into this package.
"""

from __future__ import annotations

import os
import re
from dataclasses import dataclass
from pathlib import Path


class RepoError(Exception):
    pass


# Files that must exist for this to be a pokeemerald-expansion checkout.
_MARKERS = (
    "include/data.h",
    "include/constants/opponents.h",
    "src/data.c",
    "Makefile",
)


@dataclass(frozen=True)
class Repo:
    root: Path

    @property
    def trainers_h(self) -> Path:
        return self.root / "src" / "data" / "trainers.h"

    @property
    def opponents_h(self) -> Path:
        return self.root / "include" / "constants" / "opponents.h"

    @property
    def data_h(self) -> Path:
        return self.root / "include" / "data.h"

    @property
    def backup_dir(self) -> Path:
        return self.root / "tools" / "trainer_builder" / "backups"

    def path(self, rel: str) -> Path:
        return self.root / rel

    def read(self, rel: str) -> str:
        return self.path(rel).read_text(encoding="utf-8", errors="replace")


def find_repo(start: str | os.PathLike[str] | None = None) -> Repo:
    """Walk upwards from `start` until a directory with the marker files appears."""
    here = Path(start).resolve() if start else Path(__file__).resolve()
    for candidate in [here, *here.parents]:
        if candidate.is_dir() and all((candidate / m).exists() for m in _MARKERS):
            return Repo(root=candidate)
    raise RepoError(
        f"Could not find a pokeemerald-expansion checkout at or above {here}. "
        f"Expected to find all of: {', '.join(_MARKERS)}"
    )


_DEFINE_RE = re.compile(r"^\s*#define\s+(\w+)\s+(.+?)\s*(?://.*)?$", re.MULTILINE)


def scrape_define(repo: Repo, rel: str, name: str) -> str | None:
    """Return the raw body of a single `#define NAME body`, or None."""
    try:
        text = repo.read(rel)
    except OSError:
        return None
    for match in _DEFINE_RE.finditer(text):
        if match.group(1) == name:
            return match.group(2).strip()
    return None


def detect_party_syntax(repo: Repo) -> bool:
    """True when the Makefile regenerates src/data/trainers.h from trainers.party.

    This repository sets COMPETITIVE_PARTY_SYNTAX to FALSE, which disables the
    `%.h: %.party` rule in the Makefile. That makes src/data/trainers.h the real
    source of truth and src/data/trainers.party dead weight. We check anyway so
    the tool notices if that ever flips.
    """
    value = scrape_define(repo, "include/config/general.h", "COMPETITIVE_PARTY_SYNTAX")
    return bool(value) and value.strip().upper() in ("TRUE", "1")
