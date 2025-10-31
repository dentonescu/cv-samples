# WiFiEqu (Linux) — src/wfq/wlan/

Live Wi-Fi scanner built on libnl/nl80211.

- `wifisignal.c` – worker thread that polls scans and maps frequencies to channels using the cached config.
- `wlanscan.c` – helpers for triggering nl80211 scans and collecting `wfq_signal` readings.

Back to [`../README.md`](../README.md).
