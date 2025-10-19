# WiFiEqu

Wi-Fi channel “equalizer” visualizer — a compact, cross-platform project that reads Wi-Fi scan data and displays a live bar-chart view of channel strength in the terminal or browser.

## Purpose
WiFiEqu demonstrates how system services on different operating systems can expose identical JSON APIs for local hardware data, while sharing common client visualizations.

## What it does (in plain terms)
- Scans for nearby Wi‑Fi networks and groups them by channel.
- Aggregates signal strength into a simple per‑channel bar view (“equalizer”).
- Exposes a minimal JSON API so other clients (CLI, Windows desktop, Angular web) can display the same data.

## Architecture


```
           +------------------------------------+
           |             Clients                |
           |------------------------------------|
           |  • CLI Equalizer (C)               |
           |  • Web UI (Angular)                |
           +--------------------^---------------+
                                |
                        JSON over HTTP (REST)
                                |
      +-------------------------+-------------------------+
      |                                                   |

+-----------------------+               +------------------------+
| Linux Daemon (C)      |               | Windows Service (C#)   |
|     wifiequd          |               |      WiFiEquD          |
| • Scans nearby Wi-Fi  |               | • Uses WLAN API        |
| • Aggregates RSSI     |               | • Aggregates RSSI      |
| • Serves JSON API     |               | • Serves JSON API      |
+-----------------------+               +------------------------+
```

- **Daemon (Linux, `wifiequd`)**: runs as a systemd service, polls Wi‑Fi, exposes JSON endpoints.
- **CLI client (C)**: renders a live terminal equalizer.
- **Windows service (C#)**: scans Wi-Fi and exposes JSON endpoints.
- **Web client (Angular)**: browser visualization that consumes the JSON API.

## Status
- Linux daemon and CLI scaffolding in place; Makefiles and versioning wired.
- Web/Windows clients are stubs and will iterate as the API stabilises.

## Build
```sh
make clean all              # or simply: make
```

## Install (optional)
```sh
sudo setup-user             # create 'wifiequ' user and 'wifieq' group
sudo make install           # creates user and group and installs systemd unit and binary
```

## Test & Demos
```sh
make tests                  # builds the tests
make examples               # builds the examples
make test                   # runs the unit tests
make example-demo           # runs the demos
```

> Notes: Linux build depends on the local `libdmotservices` C artifacts. See that project’s README for building the library first.

## JSON API (early sketch)
- `GET /api/v1/channels`
- `GET /api/v1/stats`

Exact schemas will be documented in `/api/README.md` as the daemon stabilises.

## Folder guide
- `linux/` — daemon and CLI (C, Makefiles, systemd unit install).
- `windows/` — Windows service (C#) (stub).
- `web-angular/` — Angular client (stub).
- `api/` — API specifications and examples.
- `VERSION` / `gen-version.py` — single source of truth for versioning.

## Roadmap
- [ ] Finalise JSON schema and add contract tests.
- [ ] Server-Sent Events (SSE) for live streaming updates.
- [ ] Windows service prototype.
- [ ] Angular visualization with channel overlays.
- [ ] Packaging and minimal installer scripts.

## License
See `/LICENSE` in the repository root.
