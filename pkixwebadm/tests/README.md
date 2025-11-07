# Tests

This directory contains the growing pytest suite for pkixwebadm.

## Structure
- `test_config.py` validates that the `Settings` loader pulls defaults and honours environment overrides.
- `test_http.py` validates the HTML error-page helper (the FastAPI/TestClient coverage is deferred until the runner issue that freezes the CLI is resolved).
- `test_libdmotservices.py` captures integration behaviour when the project consumes helpers from the sibling `libdmotservices` library.
- `conftest.py` initialises logging so debug output is visible when `pytest -o log_cli=true` is used.
- `libdmotservices/js` now ships browser helpers consumed by the UI; run `make -C ../libdmotservices/js test` (after `npm install`) so ESLint and the Node-based unit tests pass before vendoring.

Additional tests should follow the same layout; use `monkeypatch` to manage environment variables instead of depending on local `.env` files. Prefer the `app` fixture when testing routers so static mounts and dependency overrides stay consistent.

Navigate back to the [project README](../README.md) or the broader planning notes in [NOTES.md](../NOTES.md).
