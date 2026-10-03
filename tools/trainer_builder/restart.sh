#!/bin/bash
# Restart the Trainer Builder server in the background. Used during development.
PORT="${1:-8777}"
cd "$(dirname "$0")/../.." || exit 1

if command -v fuser >/dev/null 2>&1; then
  fuser -k "${PORT}/tcp" >/dev/null 2>&1
fi
sleep 1

setsid nohup python3 tools/trainer_builder/server.py --no-browser --port "$PORT" \
  > /tmp/trainer_builder.log 2>&1 < /dev/null &

sleep 6
curl -s -o /dev/null -w "server on ${PORT}: %{http_code}\n" "http://127.0.0.1:${PORT}/api/repo"
