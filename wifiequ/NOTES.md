# NOTES

Developer notes for WiFiEqu. This project stitches together a family of small components (Linux daemon, Windows service, Angular client, and shared API contracts) that all speak the same JSON dialect for channel-strength data.

## Current stage
- Linux daemon, contract, and mock/live demos are production-ready for portfolio purposes.
- Angular UI now renders the live SSE stream with channel fill for missing bins and signal-strength colouring (served via nginx with server-side stats-key injection).
- Active work targets Windows-service parity and Docker health/metrics so every client consumes the shared API consistently.

## Current File Layout
```
wifiequ/
├─ README.md                     # Project overview and usage guide
├─ NOTES.md                      # Developer log and architecture decisions
├─ LICENSE
├─ VERSION                       # Release tag
├─ Dockerfile                    # Container recipe for daemon
├─ Makefile                      # Build/test shortcuts
├─ gen-version.py                # Bumps VERSION file
├─ package.json / package-lock.json
├─ .gitattributes / .gitignore
├─ .vscode/
│  ├─ c_cpp_properties.json      # Editor configuration
│  └─ settings.json
├─ api/                          # HTTP contract and supporting assets
│  ├─ README.md
│  ├─ NOTES.md
│  ├─ openapi.yaml               # REST API specification
│  └─ img/                       # Endpoint screenshots
├─ docs/
│  ├─ README.md
│  └─ api/index.html             # Generated HTML docs
├─ dist/                         # Built binaries (gitignored in tree)
├─ docker/                       # Container entrypoints (backend)
│  └─ entrypoint-backend.sh
├─ linux/                        # Native daemon implementation (C)
│  ├─ README.md / NOTES.md / ARCHITECTURE.md
│  ├─ Makefile
│  ├─ etc/
│  │  ├─ wifiequd.conf           # Sample daemon config
│  │  └─ systemd/system/wifiequd.service
│  ├─ include/
│  │  ├─ wifiequd.h
│  │  ├─ wfq/*.h                 # Core signal interfaces
│  │  └─ wfqapi/**/*.h           # HTTP endpoint headers
│  ├─ src/
│  │  ├─ wifiequd.c
│  │  ├─ wfq/                    # Signal acquisition + config
│  │  │  ├─ config/config.c
│  │  │  └─ wlan/{wifisignal.c, wlanscan.c}
│  │  └─ wfqapi/                 # HTTP routing + endpoints
│  │     ├─ http.c / json.c / router.c
│  │     └─ endpoints/{channels.c, channels_stream.c, stats.c}
│  ├─ tests/                     # C unit tests
│  │  ├─ test_version.c
│  │  ├─ test_sample_stream.c
│  │  └─ README.md
│  ├─ examples/ex_wlanscan.c
│  └─ img/                       # Architecture diagrams, screenshots
├─ web-angular/
│  ├─ README.md
│  ├─ NOTES.md
│  ├─ Dockerfile                 # Angular build + nginx proxy with key injection
│  └─ docker/entrypoint-frontend.sh
├─ windows/
│  ├─ README.md
│  └─ NOTES.md
└─ wifiequ.logo                  # Branding asset
```

## Project Snapshot
- Shared contract: [`api/`](api/README.md) and generated docs under [`docs/`](docs/README.md) drive the payload schemas for every runtime.
- Linux reference implementation: [`linux/`](linux/README.md) already streams live or mock data via REST and SSE; it doubles as the truth source for behaviour and logs.
- Companion clients: the Angular UI and future Windows service mirror the daemon’s endpoints and consume the same DTOs so the equalizer visuals behave identically across platforms.

## Development Plan
1. **Contract first**  
   - Finalise OpenAPI definitions and JSON examples (`api/openapi.yaml`).  
   - Script schema linting and snapshot tests to guard backwards compatibility.  
   - Keep the docs regenerated via `make docs` when the contract changes.
2. **Linux daemon hardening** (current focus)  
   - Finish ring-buffer saturation tests, SSE back-pressure handling, and API key rotation helpers.  
   - Expand the CLI demos to showcase mock/live switching, labelled channels, and daemon log taps.  
   - Document tuning knobs (`refresh.millis`, `log.daemon.json`, interface overrides) in the config reference.
3. **Client parity**  
   - Windows service: mirror the `/channels`, `/stats`, and `/channels/stream` endpoints; reuse DTOs generated from the OpenAPI schema.  
   - Angular UI: already wired to the live SSE stream; next steps are health checks, metrics, and container polish.  
   - Provide integration tests (Linux daemon + clients) that assert payload compatibility.
4. **Distribution + tooling**  
   - Extend `dev.sh` to orchestrate Windows packaging steps (via WSL or documented PowerShell).  
   - Ship Docker images for the daemon + Angular UI with health checks and CI hooks (frontend container exists; add health checks next).  
   - Add metrics hooks (Prometheus-style endpoint or structured logs) once parity lands.
5. **Observability & polish**  
   - Capture scan errors and interface state changes in structured logs.  
   - Document production-ish deployment stories (systemd, Windows Service Manager, Kubernetes pod).

Iterate module by module, but keep the contract and sample payloads in sync as the single source of truth.

## Implementation Timeline
- **2025-10-12 – 2025-10-21: Core daemon scaffold**  
  Repository structure, Makefile, and daemon skeleton came together first, followed quickly by a mock signal generator that fed the polling loop. JSON logging and the v0.1.1 tag captured the first demonstrable milestone with screenshots of the mock output.
- **2025-10-22 – 2025-10-27: Hardware integration & equalizer UX**  
  Thread-safety fixes stabilised concurrent access before configuration-driven scanning entered the picture. libnl-backed wlanscan work matured into a reusable context, and by late October the libdmotservices equalizer rendered live channels with configurable interfaces and richer logging.
- **2025-10-29 – 2025-10-31: HTTP API foundation**  
  Version metadata tooling paved the way for the first OpenAPI draft. The HTTP façade brought libmicrohttpd listeners and a ring buffer to life, culminating in live `/api/v1/channels` and `/api/v1/stats` endpoints. Extensive documentation updates kept the contract and generated docs in sync.
- **2025-11-01 – 2025-11-02: Streaming, auth, and packaging**  
  Server-Sent Events completed the trio of endpoints, followed by API-key enforcement, installer helpers, CI wiring, and Docker packaging. Mock-mode safeguards ensured predictable behaviour for demos and automated runs.
- **2025-12-03: Frontend stream wiring + container split**  
  Angular UI now renders the SSE stream with channel fill and signal colouring. Added dedicated frontend nginx container with stats-key injection and adjusted compose ports (`8082` API, `8083` UI).

## Domain Notes
- Linux networking: uses `nl80211` via `libnl` to enumerate BSS info; frequencies map to human-readable channels via configuration ranges.  
- Equalizer renderer: implemented once in [`libdmotservices`](../libdmotservices/README.md) and imported by the Linux CLI demo; other clients should replicate that behaviour in their target tech.  
- Security posture: `/stats` endpoint is protected by `X-API-Key`; keep key management documented in `etc/wifiequd.conf` comments and future Windows service equivalents.  
- Mock vs. hardware: environment flag `WFQ_MOCK=1` (or `--mock` in `dev.sh`) lets tests run without a wireless card—mirror this capability for every client.

## Testing & Tooling Checkpoints
- `make test` within `linux/` runs the CMocka suite; expand with integration tests that hit the running daemon.  
- Contract validation: add CI job to lint `openapi.yaml` and diff example payloads.  
- CLI smoke scripts: keep `dev.sh --run-examples` green in mock and live modes.  
- Cross-language parity tests once the Windows and Angular builds materialise.

## Related Documentation
- Project overview: [README.md](README.md)  
- API contract: [api/README.md](api/README.md)  
- API developer notes: [api/NOTES.md](api/NOTES.md)  
- Linux implementation: [linux/README.md](linux/README.md)  
- Linux developer notes: [linux/NOTES.md](linux/NOTES.md)  
- Windows service concept: [windows/README.md](windows/README.md)  
- Windows developer notes: [windows/NOTES.md](windows/NOTES.md)  
- Angular client notes: [web-angular/README.md](web-angular/README.md)  
- Angular developer notes: [web-angular/NOTES.md](web-angular/NOTES.md)
