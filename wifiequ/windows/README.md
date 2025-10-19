# WiFiEqu — Windows Service (C#)

The Windows counterpart to the Linux daemon.  
Runs as a background service that reads local Wi-Fi channel information through the Windows WLAN API and exposes the same JSON endpoint schema as the Linux `wifiequd`.

## What it does
- Periodically scans available Wi-Fi networks.
- Aggregates signal strength per channel.
- Serves JSON endpoints such as:
  - `/api/v1/channels`
  - `/api/v1/stats`

## Build & install
Planned: Visual Studio solution for building `WiFiEquD`.  
The service will install using PowerShell or an installer script and run under a dedicated user account (not Administrator).

## Status
Service scaffolding in progress.  
JSON API compatibility and network access patterns will follow the Linux implementation.

## Related
See `../linux` for the C implementation of the daemon and shared API design notes in `../api`.
