# Linux Development Notes

## Stage
- Server mode is implemented: binds/listens, decodes framed requests, enforces API key, executes commands, and returns exit code/stdout/stderr.
- Protocol helpers are shared in `protocol.hpp/.cpp`.
- Client mode is still a stub.

## Plan
- Implement the Linux client: CLI args for host/port/key/command; send framed request, read response, print stdout/stderr/exit code.
- Add simple usage examples to `server/README.md` and `client/README.md` once client functionality lands.
- Keep tests green; consider a buffer-based decode helper to allow unit tests to run in restricted environments that block `socketpair`.

## Notes
- Build options: native `make -C linux` (no CMake) or CMake under `linux/`.
- Tests need `gtest`, `spdlog`, and `fmt`; they use `socketpair` and may be blocked in constrained sandboxes.
- HTTP is intentionally deferred; this branch focuses solely on raw TCP for clarity in the portfolio.
