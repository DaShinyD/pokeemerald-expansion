#!/bin/bash
# End-to-end save test against the running server.
#
# Edits a real trainer through the HTTP API, inspects the resulting git diff to
# confirm the change was surgical, then restores the file. Run only on a clean
# working tree.

set -u
cd "$(dirname "$0")/../.." || exit 1
PORT="${1:-8777}"
API="http://127.0.0.1:${PORT}"
FILE="src/data/trainers.h"

if [ -n "$(git status --porcelain "$FILE")" ]; then
  echo "REFUSING: $FILE already has uncommitted changes."
  exit 1
fi

echo "=== 1. validate an unmodified trainer (expect ok) ==="
curl -s "${API}/api/trainer?key=TRAINER_SAWYER_1" > /tmp/tb_sawyer.json
curl -s -X POST -H 'Content-Type: application/json' \
  --data @/tmp/tb_sawyer.json "${API}/api/validate" \
  | python3 -c 'import json,sys; r=json.load(sys.stdin); print("  ok:",r["ok"],"errors:",r["errorCount"],"warnings:",r["warningCount"])'

echo
echo "=== 2. validate a deliberately broken trainer (expect refusal) ==="
python3 - <<'PY' > /tmp/tb_bad.json
import json
d = json.load(open('/tmp/tb_sawyer.json'))
d['party'][0]['species'] = 'SPECIES_CHARMANDR'
d['party'][0]['level'] = 500
d['party'][0]['moves'] = ['MOVE_TACKLE', 'MOVE_TACKLE']
d['party'][0]['ball'] = 'ITEM_POTION'
d['name'] = 'WAYTOOLONGNAME'
json.dump(d, open('/tmp/tb_bad.json', 'w'))
PY
curl -s -X POST -H 'Content-Type: application/json' \
  --data @/tmp/tb_bad.json "${API}/api/save" > /tmp/tb_bad_result.json
python3 - <<'PY'
import json
r = json.load(open('/tmp/tb_bad_result.json'))
print("  saved:", r["ok"], "| stage:", r["stage"])
for issue in r["report"]["issues"]:
    print("    [%s] %s" % (issue["severity"], issue["message"]))
PY
echo "  git diff after refused save (should be empty):"
git diff --stat "$FILE" | sed 's/^/    /'

echo
echo "=== 3. make a real edit (level + add a move + reorder) ==="
python3 - <<'PY' > /tmp/tb_edit.json
import json
d = json.load(open('/tmp/tb_sawyer.json'))
d['party'][0]['level'] = 33
d['party'][0]['moves'] = ['MOVE_ROCK_THROW', 'MOVE_TACKLE']
d['party'][0]['heldItem'] = 'ITEM_ORAN_BERRY'
json.dump(d, open('/tmp/tb_edit.json', 'w'))
PY
curl -s -X POST -H 'Content-Type: application/json' \
  --data @/tmp/tb_edit.json "${API}/api/save" \
  | python3 -c 'import json,sys; r=json.load(sys.stdin); print("  saved:",r["ok"],"| stage:",r["stage"],"| backup:",r.get("backup"))'

echo
echo "=== 4. resulting diff ==="
git --no-pager diff --unified=3 "$FILE" | sed 's/^/    /'

echo
echo "=== 5. diff size (should be tiny) ==="
echo -n "    "; git diff --numstat "$FILE"

echo
echo "=== 6. restore ==="
git checkout -- "$FILE"
echo "    restored: $([ -z "$(git status --porcelain "$FILE")" ] && echo yes || echo NO)"
curl -s "${API}/api/reload" > /dev/null
echo "    server reloaded"
