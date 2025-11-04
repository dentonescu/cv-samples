# NOTES

Developer notes for the Angular client. These outline the milestones for bringing the browser UI in line with the daemon’s capabilities.

## Current File Layout
```
web-angular/
├─ README.md                     # Angular client vision and milestones
├─ NOTES.md                      # Developer roadmap (this file)
```

## Implementation Plan
1. **Scaffold & hygiene**  
   - Keep the Angular workspace synced with the current CLI version.  
   - Add ESLint + Prettier config to enforce linting in `npm run lint`.  
   - Establish environment files (`environment.ts`) for base URLs, mock mode, and SSE toggles.
2. **Data layer**  
   - Generate TypeScript interfaces from `api/openapi.yaml` (e.g., using `openapi-typescript`).  
   - Implement `WifiEquService` that wraps REST + SSE access, including API key injection for `/stats`.  
   - Provide a mock provider returning fixture data so Storybook/components can run without the daemon.
3. **UI components**  
   - Build responsive equalizer visuals using `ng2-charts` or a lightweight SVG renderer mirroring the CLI layout.  
   - Surface daemon metadata (refresh cadence, interface name, last scan timestamp) alongside the equalizer.  
   - Add onboarding tips for mock mode and hardware requirements.
4. **Testing & quality**  
   - Set up Jest or Karma tests for services and components; snapshot the channel bars for regression checks.  
   - Add Cypress (or Playwright) smoke tests hitting a mocked backend (use fixtures recorded from the Linux daemon).  
   - Run `npm run test`, `npm run lint`, and `npm run e2e` in CI.
5. **Build & deployment**  
   - Create a production build target in the root `Makefile` (bundled under `dist/web-angular`).  
   - Wire Dockerfile for static hosting (nginx) and add Compose entry that proxies to the daemon.  
   - Document local development flow (`ng serve`, SSE proxy configuration) and cross-origin considerations.

## Domain Notes
- SSE requires manual teardown in Angular; leverage `EventSource` wrappers with RxJS Observables and handle reconnection/backoff gracefully.  
- Balancing the equalizer: reuse channel labels from the daemon config so tooltips and legends match the CLI demo.  
- Accessibility: focus on high-contrast colours and add ARIA labels for channels so screen readers announce the signal strength.

## Testing Checklist
- `npm run lint` and `npm run test` remain green in mock and live configurations.  
- Contract snapshots compare service outputs to recorded JSON (`/channels`, `/stats`).  
- Visual regression tests capture channel bar layout across viewport sizes.

## Implementation Timeline
- **2025-10-27: workspace scaffold** — Angular README outlined client intentions and linked to the API contract. Implementation work remains ahead; future commits will document component development and SSE integration.

## Related Documentation
- Module overview: [README.md](README.md)  
- Shared contract: [../api/README.md](../api/README.md)  
- Linux reference implementation: [../linux/README.md](../linux/README.md)  
- Project-wide notes: [../NOTES.md](../NOTES.md)
