# slideshow-server tests

Pytest suite that exercises both the media helpers and the HTTP handler.

## Structure

| File | Coverage |
| --- | --- |
| `test_media.py` | Media discovery edge cases, asset loading, and `ffmpeg` command generation. |
| `test_server.py` | Handler factory behaviour, index rendering, and streaming fallbacks when files are missing. |

## Running the suite

```bash
make test                 # from project root
# or, explicitly:
python3 -m pytest -vv
```

The Makefile wires `make test` into the top-level build, so the tests run automatically during CI.

## Navigation

- Back to [`slideshow-server`](../README.md)
- Library code in [`slideshow_server`](../slideshow_server/README.md)
