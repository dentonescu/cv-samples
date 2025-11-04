# pkixwebadm package

Application code for the pkixwebadm certificate inventory service.

## Layout
- `__init__.py` – package metadata and convenience exports (`main`, `get_settings`).
- `cli.py` – temporary command-line entry point; wiring for the HTTP server will land during the “HTTP app foundation” milestone.
- `config.py` – `pydantic-settings` backed configuration loader that reads `PKIXWA_*` environment variables.
- `__main__.py` – supports `python -m pkixwebadm` by delegating to the CLI helper.

Generated artifacts (`__pycache__`, compiled bytecode) are cleaned by `make clean` and intentionally omitted from version control.

## Next steps
- Introduce the FastAPI app factory (`app.py`) and router modules discussed in [NOTES.md](../NOTES.md).
- Expand the CLI with a `serve` command once the application factory exists.

Return to the [project README](../README.md) for a broader overview.
