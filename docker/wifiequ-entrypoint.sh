#!/bin/sh
set -e

CONFIG_PATH="${WFQ_CONFIG_PATH:-/etc/wifiequd.conf}"
TOKEN_PLACEHOLDER="@@ACCESS_TOKEN_STATS@@"

if [ ! -f "$CONFIG_PATH" ]; then
    echo "Fatal: configuration file not found at $CONFIG_PATH" >&2
    exit 1
fi

if grep -q "$TOKEN_PLACEHOLDER" "$CONFIG_PATH"; then
    ACCESS_TOKEN="${WFQ_ACCESS_TOKEN:-$(openssl rand -hex 32)}"
    sed -i "s|${TOKEN_PLACEHOLDER}|${ACCESS_TOKEN}|" "$CONFIG_PATH"
    echo "Generated stats access token: $ACCESS_TOKEN"
fi

exec /usr/local/bin/wifiequd "$@"
