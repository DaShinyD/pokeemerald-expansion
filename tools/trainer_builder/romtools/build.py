"""Runs this project's real build and reports what happened.

The command is not hardcoded. `.bash_history` in the repository root shows this
project is built with `make -jN` from WSL, and `make`/`arm-none-eabi-gcc` are on
PATH there, so that is the default - but it is overridable and is reported back
to the UI so there is never any doubt about what ran.
"""

from __future__ import annotations

import os
import re
import subprocess
import time
from dataclasses import dataclass, field
from pathlib import Path

from .repo import Repo

# `file.c:123:45: error: message` - the shape gcc uses.
_DIAGNOSTIC_RE = re.compile(
    r"^(?P<file>[^\s:][^:]*):(?P<line>\d+):(?:(?P<col>\d+):)?\s*"
    r"(?P<severity>error|warning|fatal error):\s*(?P<message>.*)$",
    re.MULTILINE,
)


@dataclass
class Diagnostic:
    file: str
    line: int
    column: int | None
    severity: str
    message: str

    def to_dict(self) -> dict:
        return {
            "file": self.file,
            "line": self.line,
            "column": self.column,
            "severity": self.severity,
            "message": self.message,
        }


@dataclass
class BuildResult:
    ok: bool
    command: str
    returncode: int
    seconds: float
    output: str
    diagnostics: list[Diagnostic] = field(default_factory=list)
    note: str = ""

    def to_dict(self) -> dict:
        errors = [d for d in self.diagnostics if d.severity != "warning"]
        return {
            "ok": self.ok,
            "command": self.command,
            "returncode": self.returncode,
            "seconds": round(self.seconds, 1),
            "note": self.note,
            "errors": [d.to_dict() for d in errors],
            "warnings": [d.to_dict() for d in self.diagnostics if d.severity == "warning"],
            # The tail is what matters when a build fails; the head is mostly
            # thousands of lines of successful compiles.
            "outputTail": "\n".join(self.output.splitlines()[-400:]),
        }


def default_command(repo: Repo) -> str:
    jobs = os.cpu_count() or 4
    history = repo.root / ".bash_history"
    if history.exists():
        try:
            lines = history.read_text(encoding="utf-8", errors="replace").splitlines()
            for line in reversed(lines):
                if line.strip().startswith("make"):
                    return line.strip()
        except OSError:
            pass
    return f"make -j{jobs}"


def parse_diagnostics(output: str) -> list[Diagnostic]:
    found: list[Diagnostic] = []
    for match in _DIAGNOSTIC_RE.finditer(output):
        found.append(
            Diagnostic(
                file=match.group("file"),
                line=int(match.group("line")),
                column=int(match.group("col")) if match.group("col") else None,
                severity=match.group("severity"),
                message=match.group("message").strip(),
            )
        )
    return found


def run_build(repo: Repo, command: str | None = None, timeout: int = 3600) -> BuildResult:
    command = command or default_command(repo)
    started = time.monotonic()
    try:
        completed = subprocess.run(
            command,
            cwd=repo.root,
            shell=True,
            capture_output=True,
            text=True,
            errors="replace",
            timeout=timeout,
        )
        output = (completed.stdout or "") + (completed.stderr or "")
        returncode = completed.returncode
        note = ""
    except subprocess.TimeoutExpired:
        return BuildResult(
            ok=False, command=command, returncode=-1, seconds=time.monotonic() - started,
            output="", note=f"The build did not finish within {timeout} seconds and was stopped.",
        )
    except FileNotFoundError:
        return BuildResult(
            ok=False, command=command, returncode=-1, seconds=0, output="",
            note=f"Could not run {command!r}. Is make on PATH in this environment?",
        )

    diagnostics = parse_diagnostics(output)
    errors = [d for d in diagnostics if d.severity != "warning"]
    ok = returncode == 0 and not errors
    if not ok and not errors and not note:
        note = f"The build command exited with status {returncode} but printed no compiler errors."

    return BuildResult(
        ok=ok, command=command, returncode=returncode,
        seconds=time.monotonic() - started, output=output,
        diagnostics=diagnostics, note=note,
    )
