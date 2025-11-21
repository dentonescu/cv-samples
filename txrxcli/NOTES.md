# Developer Notes

## Intent
I am building a compact, raw-TCP demo to showcase cross-platform socket programming. The goal is to pair a Linux C++ client/server with a Windows C# client/server, all gated by a simple API key and carrying a conspicuous demo-only warning.

## Current stage
- Linux server and protocol helpers are in place; server runs and executes commands with API-key gate and captured stdout/stderr.
- Dual-mode binary scaffolding is live; client mode is still a placeholder.
- Tests exist for protocol helpers (need gtest/spdlog/fmt and a permissive sandbox for `socketpair`).
- Protocol: raw TCP with a lightweight text framing. HTTP is intentionally out of scope for this build.

## Near-term plan
- Implement the Linux client: CLI args for host/port/key/command; connect, send framed request, read response/output.
- Improve protocol helper tests so they can run in restricted environments (e.g., buffer-based decode path instead of relying on `socketpair`).
- Document the protocol and security caveats in `docs/` and subproject READMEs.

## Future work
- Add the Windows C# client/server with the same framing and banner behavior.
- Harden parsing and error handling just enough for demos (still not production-ready).
- Collect simple usage examples for both platforms.

## Screenshots
- `img/telnet-test-01.png`: manual telnet session hitting the server with API key and command framing, showing the demo-only warning and captured response. Useful as a quick visual proof the raw TCP server loop is working.
