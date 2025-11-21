# Documentation

## Navigation
- `protocol.md`: raw TCP framing, request/response examples.
- `security.md`: demo-only caveats and safe-use guidance.
- Project overview: see top-level `README.md` and `NOTES.md`.

## Status
Documentation is evolving alongside the Linux implementation. HTTP is intentionally skipped; the focus is on raw sockets. Tests need `gtest`, `spdlog`, and `fmt` dev packages and may not run in heavily restricted sandboxes.
