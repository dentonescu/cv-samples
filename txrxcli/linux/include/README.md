# Shared Headers

Headers shared between the Linux client and server:
- `protocol.hpp`: framing constants, request/response structs, and helpers to decode/encode frames and send/recv.
- `txrxcli.hpp`: lightweight logging helpers with ANSI coloring for console output.
- `ansi.hpp`: ANSI color codes.
