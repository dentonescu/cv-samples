#!/usr/bin/env bash
set -euo pipefail
if [ -z "$(which nginx)" ]; then
 echo "error: It appears nginx is not installed. Try: sudo apt install nginx"
 exit 1
fi

root="$(cd "$(dirname "$0")/.." && pwd)"
conf="$root/nginx.conf"
stats_key="$(grep -Po 'access\.token\.stats\s*=\s*\K\S+' /etc/wifiequd.conf || true)"

# defaults for local dev; allow caller to override
WFQ_BACKEND_URL="${WFQ_BACKEND_URL:-http://localhost:8080}"
WFQ_FRONTEND_PORT="${WFQ_FRONTEND_PORT:-9080}"
WFQ_STATS_KEY="${WFQ_STATS_KEY:-$stats_key}"

# temp prefix so nginx writes pid/logs under a writable dir
nginx_prefix="$(mktemp -d)"
tmp_conf="$nginx_prefix/nginx.conf"
# ensure prefix has a logs directory for the pid file
mkdir -p "$nginx_prefix/logs"
cleanup() {
  if [ -n "${nginx_pid:-}" ]; then
    nginx -c "$tmp_conf" -p "$nginx_prefix" -s quit || true
  fi
  if [ -n "$nginx_prefix" ]; then
   rm -rf "$nginx_prefix"
  fi
}
trap cleanup EXIT

# render config with current env and start nginx in foreground, but background the process so the script can continue
export WFQ_BACKEND_URL WFQ_FRONTEND_PORT WFQ_STATS_KEY
envsubst '$WFQ_BACKEND_URL $WFQ_FRONTEND_PORT $WFQ_STATS_KEY' < "$conf" > "$tmp_conf"
nginx -c "$tmp_conf" -p "$nginx_prefix" -g 'daemon off;' &
nginx_pid=$!

echo "WFQ_BACKEND_URL=$WFQ_BACKEND_URL"
echo "WFQ_FRONTEND_PORT=$WFQ_FRONTEND_PORT"
echo "nginx_pid=$nginx_pid"

# run Angular dev server
(cd "$root/wifiequ-frontend" && ng serve --port 4200 --proxy-config proxy-dev.conf.json)

wait $nginx_pid
