# Linux C++ Targets

CMake-built dual-mode binary that can run as either server or client over a simple raw-TCP protocol with API-key validation. This directory stays focused on the Linux implementation; Windows/.NET lives under `windows/`.

## Components
- `src/`: dual-mode `main.cpp` with CLI for `--mode server|client`.
- `include/`: shared headers (protocol framing, shared helpers).
- `server/` and `client/`: docs/placeholders describing the server/client behaviors.
- `NOTES.md`: development status and plans specific to Linux.

## Build
Native make (no CMake needed):
```
make -C linux
```
Or with CMake:
```
cmake -S linux -B linux/build -DCMAKE_BUILD_TYPE=Release
cmake --build linux/build --target txrxcli
```

Tests (need `gtest`, `spdlog`, `fmt`):
```
make -C linux test
```
Note: protocol tests use `socketpair` and may not run in restricted sandboxes.

## Run
```
./txrxcli --mode server --port 9000 --key SECRET
./txrxcli --mode client ...   # client mode pending implementation
```
Server mode prints the demo-only warning and executes framed requests; client mode still needs to be implemented.
