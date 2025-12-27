#!/usr/bin/env bash
set -e
SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"
DEPS_DIR="$(realpath "$SCRIPT_DIR/../../dmot-core/dep/jars")"
DMOT_CORE_LIB_DIR="$(realpath "$SCRIPT_DIR/../../dmot-core/target")"
java -cp "$DEPS_DIR/*:$DMOT_CORE_LIB_DIR/*" "$SCRIPT_DIR/APIClientExample.java"
