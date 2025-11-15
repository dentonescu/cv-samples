# NOTES

Developer notes for pkixwebadm. The aim is to keep the implementation plan visible as the project evolves.

## Architecture Snapshot
- FastAPI application using Jinja templates; Bootstrap utility classes already power the landing page and ingestion widget.
- SQLAlchemy ORM with SQLite (`app.db`) persisted on a host-mounted path when containerised.
- bcrypt for password hashing; the `pkixwebadm.security` package currently ships the native auth manager and immutable credential/identity models while the OIDC backend incubates.
- Business logic grouped in services (ingestion, expiry calculations, reminders).
- Optional background scheduler (APScheduler) to refresh remote certificate data.
- CLI is driven by `argparse` with a `--help-all` flag implemented via `libdmotservices`.

## Minimum Viable Product (MVP) definition
Containerisation and automation resume once these capabilities are in place:
- Authenticate a single bootstrap admin via session cookies, hashed credentials, and seed-user helpers (broader user management is explicitly post-MVP).
- Certificate/URL ingestion endpoints that drive the drag-and-drop + remote fetch UI.
- Persistence of certificate metadata (issuer, subject, SANs, validity window, chain ordering) into SQLite, backed by SQLAlchemy models and migrations.
- List view and calendar view that surface upcoming expirations with lightweight filtering/tagging.

## Planned File Layout
_Files marked “planned” are not yet created; others already live in the repository._
```
pkixwebadm/
├─ pkixwebadm/
│  ├─ __init__.py
│  ├─ README.md                 # Package overview
│  ├─ app.py                    # FastAPI app factory
│  ├─ cli.py                    # CLI entry point / argparse helpers
│  ├─ config.py                 # Settings loader
│  ├─ logging.py                # Logging configuration
│  ├─ security/
│  │  ├─ README.md              # Overview
│  │  ├─ credentials.py         # Auth payloads (live)
│  │  ├─ identity.py            # Authenticated principal (live)
│  │  ├─ auth/
│  │  │  ├─ README.md
│  │  │  ├─ auth_manager.py     # Shared contract (live)
│  │  │  ├─ native/             # Native backend (live)
│  │  │  └─ oidc/               # Federated backend (planned)
│  ├─ db/
│  │  ├─ __init__.py            # planned
│  │  ├─ engine.py              # Engine/session factory (planned)
│  │  ├─ base.py                # Declarative base metadata (planned)
│  │  └─ migrations/            # Alembic scripts (planned)
│  ├─ models/                   # SQLAlchemy ORM models (planned)
│  ├─ schemas/                  # Pydantic DTOs (planned)
│  ├─ services/                 # Business logic (auth, ingestion, expiry) (planned)
│  ├─ web/
│  │  ├─ views/                 # HTML routers (root view implemented)
│  │  ├─ templates/             # Jinja templates (base + ROOT live)
│  │  ├─ widgets/               # Template partials (live under templates/)
│  │  └─ static/                # CSS/JS/assets (Bootstrap palette, ingestion widget scripts, error pages)
│  ├─ background/               # APScheduler wiring (future work)
│  ├─ maintenance/              # CLI utilities (planned)
│  └─ telemetry/                # Optional observability hooks (future work)
├─ tests/
│  ├─ README.md                 # Test suite overview
│  ├─ test_config.py            # Settings loader tests
│  ├─ test_http.py              # Landing page helpers
│  ├─ test_credentials.py       # Security primitives
│  ├─ test_identity.py          # Security primitives
│  └─ test_crypto.py            # Password helpers
├─ scripts/                     # Bootstrap helpers (password generator, etc.)
├─ api/openapi.yaml             # REST contract
├─ docs/api/                    # Generated API docs
├─ alembic.ini
├─ pyproject.toml
├─ README.md / NOTES.md
├─ .env.example
├─ Dockerfile / docker-compose.yml
├─ Makefile
└─ var/data/app.db              # SQLite storage (gitignored)
```

## Development Plan
**Current stage:** HTTP + security foundation is underway; app factory, CLI, templates, and credential/identity scaffolding are done while the MVP slices (auth+ingestion+persistence+views) and post-MVP container packaging remain.

0. **Architecture review** – document overall stack, packages, and data flow. ✅
1. **Project scaffold** – create package structure, empty modules, configuration stubs, and initial dependencies. ✅
   - Manage dependencies exclusively through `pyproject.toml`; local installs use `python -m pip install -e .`. ✅
2. **HTTP + security foundation** – app factory, settings loader, static file serving, landing page widget, native auth contracts, and bootstrap script. ⏳ (MVP slices in progress; container baseline resumes afterwards).
3. **Templating layer** – extend Jinja beyond the base/landing templates; add Bootstrap-powered cards, tables, and status badges.
4. **Authentication slice (MVP)**  
   - Wire FastAPI login/logout views around the bootstrap admin credentials sourced from environment variables.  
   - Issue/validate cookie-backed sessions plus a minimal `current_user` dependency.  
   - Provide CLI utilities for regenerating the bootstrap admin password/hash.
5. **Certificate ingestion slice**  
   - Back the drag-and-drop + URL widgets with FastAPI endpoints.  
   - Parse certificates (subject, issuer, SANs, validity window, fingerprint, chain order).  
   - Persist metadata with timestamps and ownership metadata.
6. **Expiry list view** – colour-coded status (valid, expiring, expired), filters/search, and links to detail pages.
7. **Calendar view & ICS** – render calendar with expiry highlights; expose optional `.ics` feed.
8. **Background refresh (optional)** – wire APScheduler to poll endpoints and update stored metadata.
9. **Containerisation (post-MVP)** – Dockerfile, compose setup, volume-mounted SQLite path, non-root runtime user, health check.
10. **User management hardening (post-MVP)** – introduce DB-backed user/session stores, CRUD flows, and password rotation helpers beyond the bootstrap admin.

Iterate as needed, but aim to finish each slice as a demonstrable feature before moving on.

## Implementation Timeline
- **Bootstrap phase (complete):** repository created, planning notes captured, CI plumbing validated.
- **HTTP foundation (active):** settings loader, FastAPI app factory, CLI `serve` command (`--help-all`), landing page templates, and error-page fallback landed; MVP feature slices in progress and container polish resumes post-MVP.
- **Security scaffolding (active):** credential + identity models, bcrypt helpers, native `AuthManager`, and bootstrap password script delivered; bootstrap-admin wiring lands for MVP while multi-user persistence is queued post-MVP.

## Testing & QA
- Introduce pytest early (during scaffold) with a TestClient fixture for FastAPI integration tests.
- Each slice should land with coverage: parser unit tests, auth/session workflows, templated views via snapshot checks, and scheduler behaviour behind feature flags.
- Maintain lightweight smoke tests for Docker image (app starts, healthcheck passes).

## Configuration & Secrets
- Use `.env` files (key=value pairs) alongside environment variables; `.env` loaded by a settings class (e.g., `pydantic-settings`) to supply DB paths, session cookies, and optional secrets.
- Commit a `.env.example` showing required keys (sans secrets); actual `.env` ignored via `.gitignore`.
- For deployment, prefer real environment variables or orchestrator secrets; `.env` acts as local convenience.

## Data Maintenance
- Document SQLite storage location, backup cadence (copy file while app is quiesced or using `.backup` pragma), and restore process.
- Track schema revisions with Alembic migrations; include commands for `upgrade`/`downgrade`.
- Provide maintenance CLI tasks for pruning stale sessions, rehashing passwords, and checking database integrity (`PRAGMA integrity_check`).

## Future considerations
- Scheduler- and notification-related features remain intentionally deferred until the core ingestion and expiry views stabilise.
- Ingestion UI should surface validation feedback (toasts, inline errors) so drag/drop failures are obvious.
- Backend ingestion endpoints must enforce CSRF protection and server-side MIME validation before accepting uploads or remote fetches.
