# slideshow_server package

Core library backing the `slideshow-server` CLI. It exposes reusable helpers and the `main` entry point used by `python -m slideshow_server`.

## Modules

| File | Purpose |
| --- | --- |
| `__init__.py` | Loads version metadata, re-exports the public API, and keeps imports centralised so downstream callers can rely on `import slideshow_server`. |
| `__main__.py` | Allows `python -m slideshow_server` to call the CLI entry point. |
| `cli.py` | Argument parser plus the bootstrap routine that wires media discovery, HTML rendering, and the `http.server` handler together. |
| `media.py` | Discovers playable files, loads static assets, and builds the on-demand `ffmpeg` command for video streaming. |
| `server.py` | Custom `SimpleHTTPRequestHandler` subclass that serves the rendered HTML and streams video chunks through `ffmpeg`. |

## Public API

The module re-exports the pieces most callers care about:
- `main(argv=None)` – user-facing CLI entry point.
- `discover_media_files(...)` – recursive media discovery with extension filtering and symlink safeguards.
- `render_index(...)` – injects configuration constants and static assets into the HTML template.
- `create_handler(...)` / `SlideshowHandler` – HTTP handler factories for custom deployments.
- `build_ffmpeg_command(...)` – helper used by the video streaming path.

Every function carries its own docstring, so refer to the source when you need parameter details.

## Tests

The package is covered by:
- `tests/test_media.py` – media discovery rules, ffmpeg command builder, and asset loading.
- `tests/test_server.py` – handler configuration, index rendering, and streaming edge cases.

Run the suite with:

```bash
make test          # from project root
```

## Navigation

- Back to [`slideshow-server`](../README.md)
- Forward to [`tests`](../tests/README.md)
