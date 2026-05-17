#!/bin/bash

# updateCaTDD4Copilot.sh: INSTALL or UPGRADE CaTDD from MyCaTDD (local or GitHub)
# Usage: ./scripts/updateCaTDD4Copilot.sh [--source <local_dir|url>] [--init] [--clean-prompts]

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEFAULT_LOCAL="/Users/enigmawu/VSCode/MyCaTDD-1"
DEFAULT_URL="https://github.com/EnigmaWU/MyCaTDD.git"
SOURCE=""
INIT=0
CLEAN_PROMPTS=0

usage() {
    cat <<USAGE
Usage: $0 [--source <local_dir|url>] [--init] [--clean-prompts]
    --source DIR|URL   Use local clone or GitHub URL for CaTDD source.
    --init             Create target CaTDD dirs if missing.
    --clean-prompts    Clean old UT_*.prompt.md before generating.
    -h, --help         Show this help.
USAGE
}

# Parse args
while [[ $# -gt 0 ]]; do
    case "$1" in
        --source)
            [[ $# -ge 2 ]] || { echo "--source requires a path or URL" >&2; exit 2; }
            SOURCE="$2"
            shift 2
            ;;
        --init)
            INIT=1
            shift
            ;;
        --clean-prompts)
            CLEAN_PROMPTS=1
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown argument: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

# Prompt if not specified
if [[ -z "$SOURCE" ]]; then
    echo "Select CaTDD source:"
    echo "  1) Local clone: $DEFAULT_LOCAL"
    echo "  2) GitHub: $DEFAULT_URL"
    read -rp "Enter 1 or 2 [1]: " sel
    case "$sel" in
        2)
            SOURCE="$DEFAULT_URL"
            ;;
        ""|1)
            SOURCE="$DEFAULT_LOCAL"
            ;;
        *)
            echo "Invalid selection" >&2
            exit 2
            ;;
    esac
fi

MYCATDD_DIR=""
if [[ -d "$SOURCE/.git" ]]; then
    # Local clone
    MYCATDD_DIR="$SOURCE"
    echo "[INFO] Using local MyCaTDD: $MYCATDD_DIR"
    (cd "$MYCATDD_DIR" && git pull)
elif [[ "$SOURCE" =~ ^https?:// ]]; then
    # GitHub URL
    TMP_CLONE="/tmp/MyCaTDD-tmp-$$"
    rm -rf "$TMP_CLONE"
    git clone --depth=1 "$SOURCE" "$TMP_CLONE"
    MYCATDD_DIR="$TMP_CLONE"
    trap 'rm -rf "$TMP_CLONE"' EXIT
    echo "[INFO] Cloned MyCaTDD from $SOURCE"
else
    echo "[ERROR] Invalid source: $SOURCE" >&2
    exit 2
fi

# Run the installer
INSTALL_SH="$MYCATDD_DIR/scripts/installCaTDD4Copilot.sh"
if [[ ! -x "$INSTALL_SH" ]]; then
    echo "[ERROR] Installer not found: $INSTALL_SH" >&2
    exit 2
fi

INSTALL_ARGS=(--target "$PROJECT_ROOT")
[[ $INIT -eq 1 ]] && INSTALL_ARGS+=(--init)
[[ $CLEAN_PROMPTS -eq 1 ]] && INSTALL_ARGS+=(--clean-prompts)

"$INSTALL_SH" "${INSTALL_ARGS[@]}"

echo "[DONE] CaTDD is up to date in $PROJECT_ROOT"
