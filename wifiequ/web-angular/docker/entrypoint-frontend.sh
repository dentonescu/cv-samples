#!/usr/bin/env sh
set -e

# Ensure stats key is non-empty to keep nginx map happy (inject dummy if unset)
stats_key="$(grep -Po 'access\.token\.stats\s*=\s*\K\S+' /etc/wifiequd.conf 2>/dev/null || true)"
: "${WFQ_STATS_KEY:=${stats_key:-wfq-demo-token-please-change}}"

# Render nginx config with current environment
envsubst '$WFQ_BACKEND_URL $WFQ_FRONTEND_PORT $WFQ_STATS_KEY' < /etc/nginx/nginx.conf.template > /etc/nginx/nginx.conf

# Ensure runtime dirs exist
mkdir -p /var/run /var/log/nginx /etc/nginx/logs

exec nginx -g 'daemon off;'
