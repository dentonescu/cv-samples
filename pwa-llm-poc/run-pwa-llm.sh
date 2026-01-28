#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"
BACKEND_DIR="$ROOT_DIR/backend"
FRONTEND_DIR="$ROOT_DIR/frontend/llm-spa"
LOG_DIR="$ROOT_DIR/logs"
BACKEND_LOG="$LOG_DIR/backend.log"
FRONTEND_LOG="$LOG_DIR/frontend.log"

echo "Starting pwa-llm-poc in: $ROOT_DIR..."

mkdir -p "$LOG_DIR"

echo "Starting backend..."
( cd "$BACKEND_DIR" && python3 app.py ) >"$BACKEND_LOG" 2>&1 &
BACKEND_PID=$!

echo "Starting frontend..."
( cd "$FRONTEND_DIR" && npm run dev -- --config vite.config.ts ) >"$FRONTEND_LOG" 2>&1 &
FRONTEND_PID=$!

cleanup() {
  echo "Stopping processes..."
  kill "$BACKEND_PID" "$FRONTEND_PID" 2>/dev/null || true
}
trap cleanup EXIT INT TERM

echo "Backend PID: $BACKEND_PID"
echo "Frontend PID: $FRONTEND_PID"
echo "Logs:"
echo "  $BACKEND_LOG"
echo "  $FRONTEND_LOG"
echo
echo "Tailing logs (Ctrl+C to stop)..."
tail -n +1 -f "$BACKEND_LOG" "$FRONTEND_LOG"

echo "pwa-llm-poc has ended."
printf "\n\n"
