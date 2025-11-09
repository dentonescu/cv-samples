# pkixwebadm

Small self-contained certificate inventory web app that will track X.509 expirations for personal infrastructure.

## Current status
- HTTP foundation milestone is underway: the FastAPI app factory mounts static assets, renders the root template, and exposes a CLI `serve` command.
- Configuration lives in `config.Settings`; unit tests assert default/env overrides.
- Jinja templates render project metadata and surface the certificate-ingestion widget powered by the vendored libdmotservices JS helpers; when the template loader fails the app falls back to a static error page.
- `pkixwebadm.security` now exposes immutable credential/identity models plus the native authentication manager (cookie sessions). The placeholder OIDC backend lives in the same tree so future slices can fill it in without refactoring imports.
- Utility scripts (see [`scripts/`](scripts/README.md)) help bootstrap admin accounts by producing throwaway passwords + bcrypt hashes.
- Docker image layout, background schedulers, and notification channels are still being designed.

## Getting started (developer preview)
- Dependencies are tracked in `pyproject.toml`; install them into your virtual environment with `python -m pip install -e .` from this directory.
- Front-end tooling relies on Node.js; run `npm install` in this directory so ESLint can lint the vendored/libdmotservices-integrated scripts.
- Copy `.env.example` to `.env` and tailor the values (host/port, database URL, secrets). The application loads these via the `get_settings()` dependency.
- Use the CLI entry point for local runs:

  ```bash
  python3 pkixwebadm.py serve --host 127.0.0.1 --port 8080
  ```

  Add `--auto-reload` or `--no-auto-reload` to override the dev-mode toggle. For a combined help view, run `python3 pkixwebadm.py --help-all`.
- Tests currently cover the configuration loader, the inline error-page helper, and
  the vendored `libdmotservices` utility:

  ```bash
  python3 -m pytest
  ```

- Generate disposable admin credentials when seeding a local database:

  ```bash
  python3 scripts/gen_pw.py
  ```

## Why build it
Keeping TLS certificates fresh across hobby projects is still a manual, spreadsheet-driven process. pkixwebadm will ingest PEM/DER files or reach out to endpoints via `openssl s_client`, capture the full chain, and surface the data in a concise dashboard with calendar exports and reminder hooks.

## Planned milestones
1. **HTTP app foundation** ✅ _in progress_
   - [x] FastAPI app factory mounting static files.
   - [x] Root view renders `ROOT.html` with project metadata.
   - [x] CLI `serve` command and `--help-all` helper (powered by `libdmotservices`).
   - [ ] Logging configuration refinements and Docker image baseline.
2. **Ingestion & storage foundation**
   - Accept drag-and-drop uploads and local path imports for PEM/DER material.
   - Normalise certificate metadata (issuer, subject, SANs, key usage, chain depth) into SQLite via SQLAlchemy models.
   - Introduce FastAPI endpoints and server-rendered pages for viewing stored certificates.
3. **Expiry visibility**
   - Build calendar and list summaries of upcoming expirations, including filters/tags for ownership notes.
   - Generate an ICS feed (daily cron) that external calendar clients can subscribe to.
   - Add lightweight trust diagnostics (basic chain validation, OCSP probe placeholders).
4. **Automation and packaging**
   - Schedule re-checks with APScheduler to refresh endpoint certificates on a configurable cadence.
   - Publish a non-root Docker image with health checks and a bind-mounted SQLite database directory.
   - Explore optional e-mail/webhook notifications for last-mile reminders.

## Near-term research tasks
- Validate Jinja + HTMX ergonomics for the upcoming certificate tables vs. pure server-rendered views.
- Evaluate SQLite write patterns under periodic refresh jobs; confirm WAL mode keeps latency acceptable.
- Define the minimal RBAC/auth approach for an admin-only deployment (session cookie vs. token).
- Add UI validation/feedback on the landing page (toasts/snackbars for drag-and-drop errors, success states, etc.).
- Plan CSRF protections and server-side MIME-type validation ahead of wiring the ingestion endpoints.

## Navigation
- Package internals: [`pkixwebadm/README.md`](pkixwebadm/README.md)
- Web layer overview: [`pkixwebadm/web/README.md`](pkixwebadm/web/README.md)
- Security/auth scaffolding: [`pkixwebadm/security/README.md`](pkixwebadm/security/README.md)
- Tests: [`tests/README.md`](tests/README.md)
- Scripts: [`scripts/README.md`](scripts/README.md)
- Developer notes & implementation plan: [NOTES.md](NOTES.md)

## Related
- Package layout: [pkixwebadm/README.md](pkixwebadm/README.md)
- Test suite guide: [tests/README.md](tests/README.md)
- Developer notes: [NOTES.md](NOTES.md)
- Back to [portfolio overview](../README.md)
