# Developer Notes

**Navigation:** [Project README](README.md) · [Docs](docs/README.md) · [Linux notes](linux/NOTES.md) · [Windows notes](windows/NOTES.md)

## Intent
I am building a compact, raw-TCP demo to showcase cross-platform socket programming. The goal is to pair a Linux C++ client/server with a Windows C# client/server, all gated by a simple API key and carrying a conspicuous demo-only warning.

## Current stage
- Linux server and protocol helpers are in place; server runs and executes commands with API-key gate and captured stdout/stderr.
- Linux client is implemented: resolves hostnames or numeric addresses, frames requests, and prints exit code/stdout/stderr.
- Windows/.NET dual-mode app builds/publishes; server/client modes mirror the framing. Unit tests cover framing and ProcessRunner; integration coverage is still TODO.
- Protocol: raw TCP with a lightweight text framing. HTTP is intentionally out of scope for this build.

## Near-term plan
- Improve protocol helper tests so they can run in restricted environments (e.g., buffer-based decode path instead of relying on `socketpair`).
- Add integration tests for the .NET client/server (loopback listener) and consider timeouts to avoid hangs.
- Document the protocol and security caveats in `docs/` and subproject READMEs.

## Future work
- Harden parsing and error handling just enough for demos (still not production-ready).
- Collect simple usage examples for both platforms.

## Screenshots

Client access:

![txrxcli server being accessed via its client](img/client-test-01.png)

Telnet access:

![txrxcli server being accessed via Telnet](img/telnet-test-01.png)

Manual telnet session hitting the server with API key and command framing, showing the demo-only warning and captured response. Useful as a quick visual proof the raw TCP server loop is working.
