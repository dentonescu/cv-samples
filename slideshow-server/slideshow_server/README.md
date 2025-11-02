# slideshow_server package

Core library backing the `slideshow-server` CLI. It exposes reusable helpers and the `main` entry point used by `python -m slideshow_server`.

## Contents

- `__init__.py` – implements CLI argument parsing, media discovery, HTML rendering, and the HTTP handler that streams videos via `ffmpeg`.
- `__main__.py` – thin shim so the package can be launched with `python -m slideshow_server`.

Key entry points exported via `__all__`:
- `main(argv=None)` – command-line entry point.
- `create_handler(...)` – factory for a configured `SimpleHTTPRequestHandler` subclass.
- `discover_media_files(...)` – recursive media discovery with extension filtering and symlink safeguards.
- `render_index(...)` – injects configuration constants and static assets into the HTML template.

Refer to the inline docstrings for parameter details; every function includes a concise description to keep the module self-documenting.

## Tests

Unit tests exercise the package through `tests/test_slideshow_server.py`. Run them with:

```bash
make test          # from project root
```

## Navigation

- Back to [`slideshow-server`](../README.md)
- Forward to [`tests`](../tests/README.md)
