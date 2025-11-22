# Windows Targets (C#)

C# dual-mode client/server that mirrors the Linux raw-TCP protocol, API-key gate, and warning banner.

**Navigation:** [Project README](../README.md) · [Windows notes](NOTES.md)

## Build and run
- Publish win-x64 self-contained: `make -C windows` (copies `.exe` into `build/windows/`) or `cmake -S windows -B windows/build && cmake --build windows/build`.
- Run via dotnet on any platform: `make -C windows run DOTNET_ARGS="--mode server --port 9000 --key SECRET"` (or call `dotnet run --project txrxcli-dotnet/txrxcli-dotnet.csproj -- ...`).
- Usage:
  - Server: `txrxcli-dotnet.exe --mode server --port 9000 --key SECRET [--shell powershell|cmd]`
  - Client: `txrxcli-dotnet.exe --mode client --host 127.0.0.1 --port 9000 --key SECRET --cmd "dir" [--shell powershell|cmd]`

## Tests
XUnit tests live under `txrxcli-dotnet/tests`. Run with `make -C windows test` (builds then runs) or `dotnet test txrxcli-dotnet/tests/txrxcli-dotnet.Tests.csproj` (requires NuGet access to restore the test packages).
