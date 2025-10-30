# Architecture Overview

(work in progress)

WiFiEqu is built as a small pipeline that turns raw wireless scans into console-friendly visuals and JSON output.

```
nl80211 scan  -->  scanner (wlanscan.c)  -->  channel bins (config.c)
                     |                                      |
                     v                                      v
                wfq_signal[]                        wfq_config_context
                     \                                      /
                     \--> ui equalizer (libdmotservices) --
                                  |
                                  v
                         terminal + HTTP sample stream
```

## Key modules

| Area | Source | Responsibilities |
|------|--------|------------------|
| Scanner | `src/wlan/wlanscan.c` | Builds `wfq_signal` snapshots using libnl, handles scan retries, and smooths callbacks. |
| Configuration | `src/config/config.c` | Reads `etc/wifiequd.conf`, validates channel bins, and provides frequency-to-channel lookups. |
| UI integration | `examples/ex_wlanscan.c` | Bridges scanner output into the `dmot_ui_equalizer` from [libdmotservices](../../libdmotservices/README.md), including label management per channel. |
| Mock/live data | `src/mock/mocksignal.c`, `src/wlan/wifisignal.c` | Provide interchangeable data sources for demos and daemon mode. |
| HTTP façade | `src/wfqapi/http.c` | Owns the libmicrohttpd daemon, keeps a ring buffer of recent samples, and exposes monotonic read/write helpers. The router callback currently returns `MHD_NO` until endpoint wiring lands. |
| Daemon | `src/wifiequd.c` | Hosts the refresh loop, publishes JSON, interacts with the HTTP façade, and manages signal sources. |

## Configuration flow

1. `wfq_config_read` seeds channel bins with safe defaults, validates numbered keys (`ch36=5170-5190`), and caps the list when it reaches `WFQ_CONFIG_MAX_CHAN`.
2. Runtime lookups use `wfq_config_freq2chan`, which accounts for overlapping bins and returns `0` when a frequency falls outside the defined ranges.
3. Examples like `ex_wlanscan` hide unlabeled channels so the equalizer renders only what the configuration defines.

## Rendering pipeline

- `wfq_scan_wlan` produces an array of `(freq, dBm)` samples.
- Each sample is mapped to a configured channel using `wfq_config_freq2chan`.
- `dmot_ui_equalizer_set_channel_input_value` clamps the input to a safe dBm range and smooths the display to avoid jumpy bars.
- `dmot_ui_equalizer_hide_chans_without_labels` keeps the display dense by skipping unused bins, and the renderer only clears the rows it printed to preserve the rest of the terminal.

## Extensibility ideas

- Finish wiring the libmicrohttpd router so `/api/v1/channels` and the streaming endpoint can serialise straight from the ring buffer (currently stubbed).
- Expose the ring buffer over Server-Sent Events and add basic auth for remote viewers.
- Persist configuration samples so scans can be replayed offline.

## Related

- Back to [WiFiEqu (Linux)](README.md)
- [Developer notes](NOTES.md)
- [Examples](examples/README.md)
