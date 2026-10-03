#!/usr/bin/env python3
"""Local server for the Visual Trainer Builder.

Python standard library only - no pip install, no virtualenv. Runs inside WSL
where the build toolchain already lives, and serves a UI to the Windows browser
over localhost.

Run:  python3 tools/trainer_builder/server.py
"""

from __future__ import annotations

import json
import sys
import threading
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

sys.path.insert(0, str(Path(__file__).resolve().parent))

from romtools.repo import find_repo  # noqa: E402
from romtools.trainers.service import TrainerService  # noqa: E402

WEB_ROOT = Path(__file__).resolve().parent / "web"
MIME = {
    ".html": "text/html; charset=utf-8",
    ".js": "text/javascript; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".svg": "image/svg+xml",
    ".json": "application/json; charset=utf-8",
}

_service: TrainerService
_lock = threading.Lock()


class Handler(BaseHTTPRequestHandler):
    server_version = "TrainerBuilder"

    def log_message(self, fmt, *args):  # quieter than the default access log
        if "/api/" in str(args[0] if args else ""):
            sys.stderr.write(f"  {args[0]}\n")

    # -- plumbing --------------------------------------------------------

    def _send(self, status: int, body: bytes, content_type: str) -> None:
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def _json(self, payload, status: int = 200) -> None:
        self._send(status, json.dumps(payload).encode("utf-8"), "application/json; charset=utf-8")

    def _error(self, message: str, status: int = 400) -> None:
        self._json({"ok": False, "error": message}, status)

    # -- routes ----------------------------------------------------------

    def do_GET(self) -> None:  # noqa: N802
        url = urlparse(self.path)
        route, query = url.path, parse_qs(url.query)

        if route.startswith("/api/"):
            try:
                with _lock:
                    self._api(route, query)
            except Exception as exc:  # surface the real reason, never a blank 500
                self._error(f"{type(exc).__name__}: {exc}", 500)
            return

        self._static(route)

    def do_POST(self) -> None:  # noqa: N802
        route = urlparse(self.path).path
        try:
            length = int(self.headers.get("Content-Length") or 0)
            payload = json.loads(self.rfile.read(length) or b"{}")
        except (ValueError, json.JSONDecodeError) as exc:
            self._error(f"Malformed request body: {exc}")
            return

        try:
            # One edit at a time. The build runs outside the lock so the UI
            # stays responsive while make works.
            if route == "/api/validate":
                with _lock:
                    self._json(_service.validate(payload).to_dict())
            elif route == "/api/save":
                with _lock:
                    self._json(_service.save(payload))
            elif route == "/api/build":
                self._build(payload)
            else:
                self._error(f"Unknown endpoint {route}", 404)
        except Exception as exc:
            self._error(f"{type(exc).__name__}: {exc}", 500)

    def _build(self, payload: dict) -> None:
        from romtools.build import run_build

        result = run_build(_service.repo, payload.get("command") or None)
        with _lock:
            _service.reload()
        self._json(result.to_dict())

    def _api(self, route: str, query: dict) -> None:
        if route == "/api/repo":
            self._json(_service.repo_info())
        elif route == "/api/options":
            self._json(_service.options())
        elif route == "/api/trainers":
            self._json({"trainers": _service.list_trainers()})
        elif route == "/api/trainer":
            key = (query.get("key") or [""])[0]
            difficulty = (query.get("difficulty") or ["DIFFICULTY_NORMAL"])[0]
            data = _service.get(key, difficulty)
            if data is None:
                self._error(f"No trainer named {key!r} in {difficulty}.", 404)
            else:
                self._json(data)
        elif route == "/api/source":
            key = (query.get("key") or [""])[0]
            self._json({"key": key, "source": _service.source_of(key)})
        elif route == "/api/reload":
            _service.reload()
            self._json({"ok": True, **_service.repo_info()})
        elif route == "/api/build-command":
            from romtools.build import default_command

            self._json({"command": default_command(_service.repo)})
        else:
            self._error(f"Unknown endpoint {route}", 404)

    def _static(self, route: str) -> None:
        relative = "index.html" if route in ("/", "") else route.lstrip("/")
        target = (WEB_ROOT / relative).resolve()
        if not str(target).startswith(str(WEB_ROOT)) or not target.is_file():
            self._send(404, b"Not found", "text/plain; charset=utf-8")
            return
        self._send(200, target.read_bytes(), MIME.get(target.suffix, "application/octet-stream"))


def main() -> int:
    global _service

    import argparse

    cli = argparse.ArgumentParser(description="Visual Trainer Builder")
    cli.add_argument("--port", type=int, default=8777)
    cli.add_argument("--repo", default=None, help="repository root (auto-detected by default)")
    cli.add_argument("--no-browser", action="store_true")
    args = cli.parse_args()

    repo = find_repo(args.repo or Path(__file__).resolve())
    print(f"Repository : {repo.root}")
    print("Loading trainers and constants...")
    _service = TrainerService(repo)
    info = _service.repo_info()
    print(
        f"Loaded     : {info['trainerCount']} trainers, {info['partyTotal']} party members, "
        f"{info['counts']['species']} species, {info['counts']['moves']} moves"
    )
    if not info["competitivePartySyntax"]:
        print("Source     : src/data/trainers.h  (COMPETITIVE_PARTY_SYNTAX is FALSE,")
        print("             so src/data/trainers.party is NOT used by the build)")
    print(f"Free IDs   : {info['freeTrainerIdCount']} remaining below MAX_TRAINERS_COUNT")

    url = f"http://localhost:{args.port}/"
    server = ThreadingHTTPServer(("0.0.0.0", args.port), Handler)
    print(f"\nTrainer Builder running at {url}\nPress Ctrl+C to stop.\n")
    if not args.no_browser:
        threading.Timer(0.5, lambda: webbrowser.open(url)).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
