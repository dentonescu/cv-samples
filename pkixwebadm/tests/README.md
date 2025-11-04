# Tests

This directory contains the early pytest suite for pkixwebadm.

## Structure
- `test_config.py` validates that the `Settings` loader pulls defaults and honours environment overrides.
- `conftest.py` initialises logging so debug output is visible when `pytest -o log_cli=true` is used.

Additional tests should follow the same layout; use `monkeypatch` to manage environment variables instead of depending on local `.env` files.

Navigate back to the [project README](../README.md) or the broader planning notes in [NOTES.md](../NOTES.md).
