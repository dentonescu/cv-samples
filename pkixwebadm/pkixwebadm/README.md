# pkixwebadm package

Application code for the pkixwebadm certificate inventory service.

## Layout
- `__init__.py` – package metadata and convenience exports (`create_app`, `main`, `get_settings`).
- `app.py` – FastAPI app factory mounting static assets and wiring the root router.
- `cli.py` – command-line entry point with `serve`, `--help-all`, and CLI-to-uvicorn wiring.
- `config.py` – `pydantic-settings` backed configuration loader that reads `PKIXWA_*` environment variables.
- `logging.py` – shared logging configuration invoked by the app factory.
- `security/` – credentials, identity models, and auth backends (native + placeholder OIDC).
- `web/` – view routers, templates, static assets, and error page helpers.
- `__main__.py` – supports `python -m pkixwebadm` by delegating to the CLI helper.

Generated artifacts (`__pycache__`, compiled bytecode) are cleaned by `make clean` and intentionally omitted from version control.

## Next steps
- Flesh out the database layer (SQLAlchemy models + migrations) ahead of ingestion.
- Add the certificate ingestion APIs plus UI feedback loops described in [NOTES.md](../NOTES.md).

Return to the [project README](../README.md) for a broader overview.
