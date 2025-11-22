# Dual-Mode Binary

**Navigation:** [Linux README](../README.md) · [Linux notes](../NOTES.md)

`txrxcli` runs as either client or server based on `--mode client|server`. Both branches print the demo-only warning on startup. Server mode is implemented; client mode is still pending.

## Usage (current)
- Server: `./txrxcli --mode server --port 9000 --key SECRET`
- Client: `./txrxcli --mode client --host 127.0.0.1 --port 9000 --key SECRET --cmd "ls -l"`
