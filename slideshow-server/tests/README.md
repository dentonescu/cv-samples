# slideshow-server tests

The `tests` directory houses the pytest suite that validates the slideshow server package.

## Structure

- `test_slideshow_server.py` — end-to-end coverage for media discovery, HTML rendering, handler configuration, ffmpeg streaming, and error handling.

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
