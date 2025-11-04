# NOTES

Developer notes for the WiFiEqu API contract. Keep this file aligned with `openapi.yaml` so every module implements the same surface.

## Current File Layout
```
api/
├─ README.md                     # How the spec is maintained and consumed
├─ NOTES.md                      # Contract stewardship plan (this file)
├─ openapi.yaml                  # Authoritative REST specification
└─ img/                          # Endpoint screenshots and diagrams
```

## Contract Stewardship Plan
1. **Spec governance**  
   - Treat `openapi.yaml` as the authoritative definition; regenerate docs after each change (`make docs`).  
   - Introduce an API changelog summarising additions, breaking changes, and deprecations (appendix at the bottom of the spec).  
   - Add Spectral linting (`npx @redocly/cli lint`) to CI to prevent schema drift.
2. **Example fidelity**  
   - Maintain JSON fixtures in `examples/` rooted in real daemon payloads (mock + live).  
   - Validate examples against the schema during CI (e.g., using `ajv` or `openapi-examples-validator`).  
   - Share fixtures with the Angular client and Windows service tests to keep contract parity.
3. **Client generation**  
   - Provide codegen recipes (`scripts/gen-ts.sh`, `scripts/gen-cs.ps1`) producing DTOs for TypeScript and C#.  
   - Version generated clients alongside the spec (e.g., tag `v0.2.0` when the schema bumps).  
   - Document manual tweaks (naming overrides, union handling) so regeneration is repeatable.
4. **Testing**  
   - Add contract tests that exercise the running Linux daemon (`curl` → `jq` assertions) and compare with fixtures.  
   - When Windows/Angular implementations land, introduce cross-implementation compatibility tests using the same Postman or Newman collection.  
   - Track spec coverage metrics (tests per endpoint).
5. **Publishing**  
   - Keep the generated HTML available in `../docs/api/index.html` and automate deployment to `cv-samples.vercel.app`.  
   - Provide a CLI snippet for developers: `make docs && open docs/api/index.html`.

## Domain Notes
- `/api/v1/channels` returns the most recent aggregate array; order is stable and channel numbers map to config bins.  
- `/api/v1/stats` surfaces daemon metadata (startup timestamp, refresh cadence, interface name) and is protected via `X-API-Key`.  
- `/api/v1/channels/stream` streams `text/event-stream` payloads; keep SSE event names consistent (`channels`) for client reuse.

## Implementation Timeline
- **2025-10-29: Specification groundwork**  
  Version tooling firmed up just ahead of the first OpenAPI draft and its companion Markdown/HTML docs. Automated doc-generation scripts ensured the hosted preview stayed current from the outset.
- **2025-10-31: Endpoint rollout & documentation cadence**  
  `/api/v1/channels` went live, the spec captured the public versus authenticated surfaces, and regenerated assets confirmed contract alignment.
- **2025-11-01: Streaming + auth alignment**  
  The SSE endpoint and API-key enforcement triggered another round of spec updates and architecture documentation, cementing the current v1 surface.

## Related Documentation
- API overview: [README.md](README.md)  
- Project-wide notes: [../NOTES.md](../NOTES.md)  
- Linux implementation: [../linux/README.md](../linux/README.md)
