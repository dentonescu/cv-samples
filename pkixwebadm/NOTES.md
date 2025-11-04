# NOTES

Developer notes for pkixwebadm. The aim is to keep the implementation plan visible as the project evolves.

## Architecture Snapshot
- FastAPI application using Jinja templates; Bootstrap for first-pass styling.
- SQLAlchemy ORM with SQLite (`app.db`) persisted on a host-mounted path when containerised.
- Passlib/bcrypt for password hashing; signed session cookie for authentication.
- Business logic grouped in services (ingestion, expiry calculations, reminders).
- Optional background scheduler (APScheduler) to refresh remote certificate data.

## Planned File Layout
_Files marked “planned” are not yet created; others already live in the repository._
```
pkixwebadm/
├─ pkixwebadm/
│  ├─ __init__.py
│  ├─ README.md                 # Package overview
│  ├─ app.py                     # FastAPI app factory (planned)
│  ├─ config.py                  # Settings loader
│  ├─ logging.py                 # Logging configuration (planned)
│  ├─ db/
│  │  ├─ __init__.py             # planned
│  │  ├─ engine.py               # Engine/session factory (planned)
│  │  ├─ base.py                 # Declarative base metadata (planned)
│  │  └─ migrations/             # Alembic scripts (planned)
│  ├─ models/                    # SQLAlchemy ORM models (planned)
│  ├─ schemas/                   # Pydantic DTOs (planned)
│  ├─ services/                  # Business logic (auth, ingestion, expiry) (planned)
│  ├─ web/
│  │  ├─ api/                    # JSON routers (planned)
│  │  ├─ views/                  # HTML routers (planned)
│  │  ├─ templates/              # Jinja templates (planned)
│  │  └─ static/                 # CSS/JS/assets (planned)
│  ├─ background/                # APScheduler wiring (future work)
│  ├─ security/                  # Hashing/sessions/dependencies (planned)
│  ├─ maintenance/               # CLI utilities (planned)
│  └─ telemetry/                 # Optional observability hooks (future work)
├─ tests/
│  ├─ README.md                  # Test suite overview
│  └─ test_config.py             # Settings loader tests
├─ api/openapi.yaml              # REST contract
├─ docs/api/                     # Generated API docs
├─ alembic.ini
├─ pyproject.toml
├─ README.md / NOTES.md
├─ .env.example
├─ Dockerfile / docker-compose.yml
├─ Makefile
└─ var/data/app.db               # SQLite storage (gitignored)
```

## Development Plan
**Current stage:** Milestone 2 (HTTP app foundation) is in progress; configuration scaffolding and initial tests are in place.

0. **Architecture review** – document overall stack, packages, and data flow. ✅
1. **Project scaffold** – create package structure, empty modules, configuration stubs, and initial dependencies. ✅
   - Manage dependencies exclusively through `pyproject.toml`; local installs use `python -m pip install -e .`. ✅
2. **HTTP app foundation** – app factory, settings loader, static file serving, basic landing page with project name/version. _In progress (settings + tests delivered; FastAPI app factory pending)._
3. **Templating layer** – integrate Jinja, add a base template, and verify rendering pipeline.
4. **Authentication slice**  
   - Design user/session tables and Pydantic schemas.  
   - Implement config for SQLite path and secret keys (env vars or `.env`).  
   - Build login/logout views, password hashing, session cookie handling, and `current_user` dependency.  
   - Provide CLI utilities for user creation, password updates, and deactivation.
5. **Certificate ingestion slice**  
   - UI/drag-and-drop + API endpoint to upload PEM or trigger host fetch.  
   - Parse certificates (subject, issuer, SANs, validity window, fingerprint, chain order).  
   - Persist metadata with “added at” timestamps and “added by” references.
6. **Expiry list view** – colour-coded status (valid, expiring, expired), filters/search, and links to detail pages.
7. **Calendar view & ICS** – render calendar with expiry highlights; expose optional `.ics` feed if effort stays low.
8. **Background refresh (optional)** – wire APScheduler to poll endpoints and update stored metadata.
9. **Containerisation** – Dockerfile, compose setup, volume-mounted SQLite path, non-root runtime user, health check.

Iterate as needed, but aim to finish each slice as a demonstrable feature before moving on.

## Implementation Timeline
- **2025-11-02: Repository bootstrap** — Initial commit framed the certificate inventory concept and seeded pipeline configuration. Follow-up changes added SAST/secret detection templates and captured early planning notes. Application code is still forthcoming, so the current focus remains on solidifying the architecture plan before scaffolding.

## Testing & QA
- Introduce pytest early (during scaffold) with a TestClient fixture for FastAPI integration tests.
- Each slice should land with coverage: parser unit tests, auth/session workflows, templated views via snapshot checks, and scheduler behaviour behind feature flags.
- Maintain lightweight smoke tests for Docker image (app starts, healthcheck passes).

## Configuration & Secrets
- Use `.env` files (key=value pairs) alongside environment variables; `.env` loaded by a settings class (e.g., `pydantic-settings`) to supply DB paths, session cookies, and optional secrets.
- Commit a `.env.example` showing required keys (sans secrets); actual `.env` ignored via `.gitignore`.
- For deployment, prefer real environment variables or orchestrator secrets; `.env` acts as local convenience.

## Logging & Telemetry
- Centralise logging setup in the app factory using Python’s `logging` module (`dictConfig` with JSON/text format).
- Standardise log fields (request id, user id, certificate id) so downstream analysis is consistent.
- Telemetry expectations: HTTP request logging, certificate ingestion outcomes, scheduler job results, and warn-level alerts for imminent expiries; consider optional hooks for metrics/export later.

## Data Maintenance
- Document SQLite storage location, backup cadence (copy file while app is quiesced or using `.backup` pragma), and restore process.
- Track schema revisions with Alembic migrations; include commands for `upgrade`/`downgrade`.
- Provide maintenance CLI tasks for pruning stale sessions, rehashing passwords, and checking database integrity (`PRAGMA integrity_check`).

## Future considerations
- Scheduler- and notification-related features remain intentionally deferred until the core ingestion and expiry views stabilise.
