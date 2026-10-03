#!/bin/bash
# Triggers a build through the running server and summarises the result.
cd "$(dirname "$0")/../.." || exit 1
PORT="${1:-8777}"

echo "Starting build via the API (this uses the project's real build command)..."
curl -s -X POST -H 'Content-Type: application/json' -d '{}' \
  --max-time 3000 "http://127.0.0.1:${PORT}/api/build" > /tmp/tb_build.json

python3 - <<'PY'
import json
r = json.load(open('/tmp/tb_build.json'))
print("ok        :", r["ok"])
print("command   :", r["command"])
print("seconds   :", r["seconds"])
print("exit code :", r["returncode"])
print("errors    :", len(r["errors"]))
print("warnings  :", len(r["warnings"]))
if r["note"]:
    print("note      :", r["note"])
for d in r["errors"][:10]:
    print("   ", "%s:%s" % (d["file"], d["line"]), d["message"])
print("--- output tail ---")
print("\n".join(r["outputTail"].splitlines()[-12:]))
PY
