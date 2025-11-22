# txrxcli

An intentionally small, raw-TCP demo that lets a Linux box and a Windows box send authenticated commands to each other for learning purposes only. It is **not** hardened, **not** for production, and should only be run on an isolated, trusted network.

## What lives here
- `linux/`: C++ dual-mode client/server binary (raw TCP, API-key gated). Server and client modes are working.
- `windows/`: C#/.NET dual-mode client/server mirror.
- `docs/`: protocol notes, security caveats, and usage examples.
- `NOTES.md`: personal plan/status for this portfolio project.

## Navigation
- [Linux README](linux/README.md)
- [Windows README](windows/README.md)
- [Docs](docs/README.md)
- [Developer notes](NOTES.md)

## Safety first
This project executes arbitrary commands. Use throwaway machines or VMs on a closed network. Do **not** expose it to the internet or rely on it for remote administration. It exists solely to show cross-platform socket programming and simple authentication.

## Build and test (Linux)
- Native make (no CMake): `make -C linux`
- CMake alternative: `cmake -S linux -B linux/build -DCMAKE_BUILD_TYPE=Release && cmake --build linux/build --target txrxcli`
- Tests (require `libgtest-dev`, `libspdlog-dev`, `libfmt-dev`): `make -C linux test`  
  Tests use `socketpair` and may be blocked in restricted sandboxes.

## Build and test (Windows/.NET)
- Make: `make -C windows` to publish a win-x64 self-contained `.exe` into `build/windows/`.
- CMake: `cmake -S windows -B windows/build -G Ninja && cmake --build windows/build` (targets `txrxcli_dotnet_publish` by default).
- Run via dotnet (e.g., on Linux): `make -C windows run DOTNET_ARGS="--mode server --port 9000 --key SECRET"`.
- Tests (xUnit): `make -C windows test` (builds then runs) or `dotnet test windows/txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj` (requires NuGet access to restore test packages).

## Screenshots

Server running in a Windows VM:

![screenshot of txrxcli's server mode in Windows](img/win-powershell-server-01.png)

Remote execution using a Linux client:

![screenshot of remote execution using txrxcli client in Linux](img/linux-client-01.png)
