#!/usr/bin/env bash
# Manual test harness for AutoRelay.
#
# Usage:
#   ./run_manual_test.sh [path/to/AutoRelay] [--no-reset]
#
# Default binary path: ../build/AutoRelay (i.e. a CMake build run from repo root)
#
# By default this wipes tests/sandbox/ and starts fresh on every run.
# Pass --no-reset to re-copy fixtures into the existing sandbox/source
# without clearing sandbox/target first -- this exercises FileMover's
# duplicate-filename rename logic, since the same filenames will already
# exist in the target from the previous run.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SANDBOX_DIR="$SCRIPT_DIR/sandbox"
SOURCE_DIR="$SANDBOX_DIR/source"
TARGET_DIR="$SANDBOX_DIR/target"

RESET=1
BINARY="$SCRIPT_DIR/../build/AutoRelay"

for arg in "$@"; do
    case "$arg" in
        --no-reset) RESET=0 ;;
        *) BINARY="$arg" ;;
    esac
done

if [ ! -x "$BINARY" ]; then
    echo "Error: AutoRelay binary not found or not executable at: $BINARY"
    echo "Build it first (e.g. 'cmake -B build && cmake --build build') or pass the path explicitly:"
    echo "  ./run_manual_test.sh /path/to/AutoRelay"
    exit 1
fi

if [ "$RESET" -eq 1 ]; then
    echo "== Resetting sandbox =="
    rm -rf "$SANDBOX_DIR"
    mkdir -p "$SOURCE_DIR" "$TARGET_DIR"

    echo "== Copying fixtures into sandbox/source =="
    cp "$SCRIPT_DIR"/fixtures/* "$SOURCE_DIR"/

    echo "== Backdating a few files to test date-subfolder sorting =="
    touch -d "2024-03-15" "$SOURCE_DIR/resume.pdf"
    touch -d "2023-11-02" "$SOURCE_DIR/contract.docx"
    touch -d "2025-07-20" "$SOURCE_DIR/photo.jpg"

    echo "== Copying the AutoRelay binary into source and running THAT copy =="
    echo "   (simulates the original bug: running AutoRelay from inside the"
    echo "   folder it's about to organize)"
    cp "$BINARY" "$SOURCE_DIR/AutoRelay"
    RUN_BINARY="$SOURCE_DIR/AutoRelay"
else
    echo "== Re-using existing sandbox (--no-reset): re-copying fixtures to test duplicate-name handling =="
    mkdir -p "$SOURCE_DIR" "$TARGET_DIR"
    cp "$SCRIPT_DIR"/fixtures/* "$SOURCE_DIR"/
    RUN_BINARY="$BINARY"
fi

echo "== Running AutoRelay =="
"$RUN_BINARY" "$SOURCE_DIR" "$TARGET_DIR"

echo
echo "== source/ after run (only the binary should remain) =="
ls "$SOURCE_DIR"

echo
echo "== target/ tree =="
find "$TARGET_DIR" -type f | sort