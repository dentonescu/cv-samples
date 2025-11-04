# NOTES

Developer notes for the Windows service (`WiFiEquD`). These outline how the service should converge with the Linux daemon while respecting Windows-specific constraints.

## Current File Layout
```
windows/
├─ README.md                     # Windows service concept and goals
├─ NOTES.md                      # Developer roadmap (this file)
```

## Implementation Plan
1. **Service scaffold**  
   - Create a Windows Service project (C#/.NET 8) with a hosted worker that boots via `HostApplicationBuilder`.  
   - Provide PowerShell scripts for install/uninstall and wrap them in `dev.ps1` so the root `dev.sh` can call into WSL when needed.  
   - Define a dedicated service account with minimal privileges and document required capabilities (WLAN API access).
2. **Wi-Fi sampling layer**  
   - Use `WlanOpenHandle`, `WlanEnumInterfaces`, and `WlanGetNetworkBssList` to mirror the Linux scanner output.  
   - Map frequencies to channels using the same configuration table as the daemon; keep the lookup centralised (shared JSON or generated C# constants).  
  - Build a mock source toggled via configuration flag so integration tests run without hardware.
3. **HTTP façade parity**  
   - Self-host Kestrel on `http://localhost:8080` (configurable) and expose `/api/v1/channels`, `/api/v1/stats`, and `/api/v1/channels/stream`.  
   - Reuse DTOs generated from `api/openapi.yaml` (e.g., via NSwag) so payloads remain identical to the Linux implementation.  
   - Honour the `X-API-Key` requirement for `/stats` and log access attempts with structured fields.
4. **Observability & ops**  
   - Surface Windows Event Log entries in the same structure as the daemon’s JSON logs; optionally add Serilog sinks for file-based logging.  
   - Provide readiness checks and sample Event Viewer filters in the docs.  
   - Add integration tests that spin up the service in-process and call each endpoint with mock data.
5. **Packaging**  
   - Produce a zipped `PublishSingleFile` artifact with install scripts.  
   - Document prerequisites (WLAN AutoConfig service, .NET runtime) and upgrade steps.  
   - Explore MSI or WiX packaging only after the service stabilises.

## Domain Notes
- WLAN Native Wi-Fi API requires the “Native Wifi” imports from `wlanapi.dll`; wrap them in safe handles and `using` patterns to avoid leaks.  
- Windows Service control flow needs graceful shutdown hooks (`OnStop`); ensure the SSE stream completes promptly on stop signals.  
- Keep the configuration story parallel to `etc/wifiequd.conf` (likely JSON or YAML). Provide a sample config under `windows/etc/wifiequd.windows.json`.

## Testing Checklist
- Unit tests around frequency-to-channel mapping and DTO serialisation.  
- Integration suite that hosts the service with mock Wi-Fi data and asserts endpoint parity against recorded Linux fixtures.  
- PowerShell script smoke test (`./scripts/install.ps1 --dry-run`) that validates permissions and path resolution.  
- CI job running on Windows runner to build, test, and publish artifacts.

## Implementation Timeline
- **2025-10-27: documentation stub** — Windows service README established expectations and linked back to the Linux reference. No code has landed yet; upcoming commits will track the service scaffold once development starts.

## Related Documentation
- Service overview: [README.md](README.md)  
- Shared contract: [../api/README.md](../api/README.md)  
- Linux reference implementation: [../linux/README.md](../linux/README.md)
