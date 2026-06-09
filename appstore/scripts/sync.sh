#!/bin/bash
# Sync App Store data from upstream server
# Usage: ./sync.sh [upstream_url] [appstore_url]

set -euo pipefail

UPSTREAM="${1:-http://ghp.iceis.co.uk}"
APPSTORE_URL="${2:-https://appstore.voltbin.xyz}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
WWW_DIR="${SCRIPT_DIR}/../www"

echo "=== HeavyButter App Store Sync ==="
echo "Upstream:  ${UPSTREAM}"
echo "Our URL:   ${APPSTORE_URL}"
echo "Target:    ${WWW_DIR}"
echo ""

# 1. Download categories.json
echo "[1/5] Fetching categories..."
curl -sL "${UPSTREAM}/service/main/releases/categories.json" \
  -o "${WWW_DIR}/service/main/releases/categories.json"

# 2. Parse categories and download each category file
echo "[2/5] Fetching category data..."
CATEGORIES=$(python3 -c "
import json
with open('${WWW_DIR}/service/main/releases/categories.json') as f:
    data = json.load(f)
for cat in data['categories']:
    print(cat['slug'])
")

for slug in $CATEGORIES; do
    echo "  -> category-${slug}.min.json"
    curl -sL "${UPSTREAM}/service/main/releases/category-${slug}.min.json" \
      -o "${WWW_DIR}/service/main/releases/category-${slug}.min.json"
done

# 3. Download and patch the App Store JS
echo "[3/5] Fetching and patching App Store JS..."
mkdir -p "${WWW_DIR}/service/appstore"
UPSTREAM_JS=$(curl -sL "${UPSTREAM}/service/appstore/")
PATCHED_JS="${UPSTREAM_JS//\"http://ghp.iceis.co.uk\"/\"${APPSTORE_URL}\"}"
echo "${PATCHED_JS}" > "${WWW_DIR}/service/appstore/index.js"
echo "  -> H constant set to: ${APPSTORE_URL}"
echo "[3.5/5] Rebranding apps for SharkSoup HeavyButter..."
python3 "${SCRIPT_DIR}/rebrand.py" "${WWW_DIR}"
echo ""

# 4. Compute and display SHA-256
echo "[4/5] Computing integrity hash..."
SHA=$(sha256sum "${WWW_DIR}/service/appstore/index.js" | cut -d' ' -f1)
echo "  -> SHA-256: ${SHA}"
echo ""
echo "  IMPORTANT: Update EXPECTED_APPSTORE_SHA256 in"
echo "  src/core/settings.cpp with this value:"
echo "  #define EXPECTED_APPSTORE_SHA256 \"${SHA}\""

# 5. Scan categories for repository metadata
echo ""
echo "[5/5] Fetching repository metadata..."
for slug in $CATEGORIES; do
    FILE="${WWW_DIR}/service/main/releases/category-${slug}.min.json"
    python3 -c "
import json, os, subprocess, sys
upstream = sys.argv[1]
with open('${FILE}') as f:
    data = json.load(f)
for app in data.get('apps', []):
    slug = app.get('s', '')
    if slug:
        name = slug.replace(' ', '%20')
        url = f'{upstream}/service/main/repositories/{name}/metadata.json'
        path = '${WWW_DIR}/service/main/repositories/{}/metadata.json'.format(slug)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        if not os.path.exists(path):
            result = subprocess.run(
                ['curl', '-sL', '-o', path, '--fail', url],
                capture_output=True, timeout=30
            )
            if result.returncode == 0:
                print(f'  + {slug}')
" "${UPSTREAM}"
done

echo ""
echo "=== Sync Complete ==="
echo "Run 'docker compose up -d' to start services."
