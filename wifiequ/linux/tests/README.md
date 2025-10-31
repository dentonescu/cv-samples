# WiFiEqu (Linux) — tests/

Houses unit and integration tests for the Linux build.

- `test_sample_stream.c` validates the HTTP ring buffer.
- `test_version.c` ensures version helpers align with `VERSION`.

Run them via `make test` or individually with CMocka.

Back to the [Linux docs](../README.md).
