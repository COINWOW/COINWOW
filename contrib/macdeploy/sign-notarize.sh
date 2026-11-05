#!/usr/bin/env bash
# Sign, notarize and package COINWOW-Qt.app on macOS.
#
# Usage:
#   contrib/macdeploy/sign-notarize.sh /path/to/COINWOW-Qt.app [output.zip]
#
# One-time setup (on the Mac that holds the Developer ID certificate):
#   xcrun notarytool store-credentials "coinwow-notary"
# The Apple ID password never leaves the Keychain. Nothing secret is read from
# or written to this repository.
#
# Optional environment variables:
#   COINWOW_SIGN_IDENTITY  codesign identity (default: "Developer ID Application")
#   COINWOW_NOTARY_PROFILE notarytool keychain profile (default: "coinwow-notary")

set -euo pipefail

IDENTITY="${COINWOW_SIGN_IDENTITY:-Developer ID Application}"
PROFILE="${COINWOW_NOTARY_PROFILE:-coinwow-notary}"

die() { echo "error: $*" >&2; exit 1; }
# Hide team identifiers from anything printed to the terminal.
mask() { sed -E 's/\([A-Z0-9]{10}\)/(XXX)/g; s/(TeamIdentifier=).*/\1XXX/'; }

[ "$(uname -s)" = "Darwin" ] || die "this script must run on macOS"
[ $# -ge 1 ] && [ $# -le 2 ] || die "usage: $0 /path/to/COINWOW-Qt.app [output.zip]"

APP="${1%/}"
[ -d "$APP" ] || die "app not found: $APP"
OUT="${2:-$(dirname "$APP")/COINWOW-Core-macOS.zip}"

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

# Work on a copy so the original build output is never modified.
APP_COPY="$WORK/$(basename "$APP")"
ditto "$APP" "$APP_COPY"

echo "==> Signing (hardened runtime, secure timestamp)"
codesign --deep --force --options runtime --timestamp --sign "$IDENTITY" "$APP_COPY"
codesign --verify --deep --strict --verbose=2 "$APP_COPY"

echo "==> Uploading to Apple for notarization (this can take a while)"
SUBMIT_ZIP="$WORK/notarize.zip"
ditto -c -k --keepParent "$APP_COPY" "$SUBMIT_ZIP"
RESULT="$(xcrun notarytool submit "$SUBMIT_ZIP" --keychain-profile "$PROFILE" --wait 2>&1)" \
    || { echo "$RESULT" | mask; die "notarization submission failed"; }
echo "$RESULT" | mask

if ! echo "$RESULT" | grep -q "status: Accepted"; then
    ID="$(echo "$RESULT" | awk '/^ *id:/ {print $2; exit}')"
    echo "Notarization was not accepted. For details run:" >&2
    echo "  xcrun notarytool log $ID --keychain-profile \"$PROFILE\"" >&2
    exit 1
fi

echo "==> Stapling the notarization ticket"
xcrun stapler staple "$APP_COPY"
xcrun stapler validate "$APP_COPY"
spctl -a -vv -t exec "$APP_COPY" 2>&1 | mask

echo "==> Packaging $OUT"
STAGE="$WORK/stage"
mkdir -p "$STAGE"
ditto "$APP_COPY" "$STAGE/$(basename "$APP_COPY")"
[ -f "$REPO_ROOT/COPYING" ] && cp "$REPO_ROOT/COPYING" "$STAGE/COPYING"
rm -f "$OUT"
ditto -c -k "$STAGE" "$OUT"

echo "==> Done"
shasum -a 256 "$OUT"
