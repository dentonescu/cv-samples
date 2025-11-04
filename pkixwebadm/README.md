# pkixwebadm (planning)

Small self-contained certificate inventory web app that will track X.509 expirations for personal infrastructure.

## Current status
- Scope and technical stack defined; implementation has not started yet.
- Target stack: FastAPI + SQLAlchemy on SQLite for a portable data store, with Bootstrap-backed templates for the first UI.
- Docker image layout, background schedulers, and notification channels are being designed alongside the initial backlog.

## Getting started (developer preview)
- Dependencies are tracked in `pyproject.toml`; install them into your virtual environment with `python -m pip install -e .` from this directory.
- Copy `.env.example` to `.env` and tailor the values (host/port, database URL, secrets). The application will load these through the forthcoming settings module.
- Until the FastAPI app factory lands, the CLI stub is the primary entry point; expect `serve` and related commands to arrive during the “HTTP app foundation” milestone.

## Why build it
Keeping TLS certificates fresh across hobby projects is still a manual, spreadsheet-driven process. pkixwebadm will ingest PEM/DER files or reach out to endpoints via `openssl s_client`, capture the full chain, and surface the data in a concise dashboard with calendar exports and reminder hooks.

## Planned milestones
1. **Ingestion & storage foundation**
   - Accept drag-and-drop uploads and local path imports for PEM/DER material.
   - Normalise certificate metadata (issuer, subject, SANs, key usage, chain depth) into SQLite via SQLAlchemy models.
   - Introduce FastAPI endpoints and server-rendered pages for viewing stored certificates.
2. **Expiry visibility**
   - Build calendar and list summaries of upcoming expirations, including filters/tags for ownership notes.
   - Generate an ICS feed (daily cron) that external calendar clients can subscribe to.
   - Add lightweight trust diagnostics (basic chain validation, OCSP probe placeholders).
3. **Automation and packaging**
   - Schedule re-checks with APScheduler to refresh endpoint certificates on a configurable cadence.
   - Publish a non-root Docker image with health checks and a bind-mounted SQLite database directory.
   - Explore optional e-mail/webhook notifications for last-mile reminders.

## Near-term research tasks
- Validate FastAPI + Jinja templating ergonomics for the intended flows vs. a small HTMX layer.
- Evaluate SQLite write patterns under periodic refresh jobs; confirm WAL mode keeps latency acceptable.
- Define the minimal RBAC/auth approach for an admin-only deployment (session cookie vs. token).

## Related
- Package layout: [pkixwebadm/README.md](pkixwebadm/README.md)
- Test suite guide: [tests/README.md](tests/README.md)
- Developer notes: [NOTES.md](NOTES.md)
- Back to [portfolio overview](../README.md)
