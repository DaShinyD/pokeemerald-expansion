"""Shared repository-aware tooling for this pokeemerald-expansion project.

Layout is deliberately tool-agnostic so later editors (encounters, shops, ...)
can reuse the same foundation:

    romtools/repo.py       - locate the checkout, read config flags
    romtools/constants.py  - scrape live constant tables from the headers
    romtools/build.py      - run the project's real build command
    romtools/trainers/     - the trainer tool: model, parser, validator, writer
"""

__all__ = ["repo", "constants", "build", "trainers"]
