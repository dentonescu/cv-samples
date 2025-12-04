# NOTES

Developer notes for the Angular client. These outline the milestones for bringing the browser UI in line with the daemon’s capabilities.

## Current stage
- UI renders live `/api/v1/channels/stream` data with signal colouring and fills missing bins from the backend config.
- nginx proxy injects the stats API key from `WFQ_STATS_KEY`; dev proxy config exists for local runs.
- Container image builds the Angular app and serves it via nginx (`wifiequ-frontend` in Compose).

## Current File Layout
```
web-angular/
├─ README.md                     # Angular client status and usage
├─ NOTES.md                      # Developer roadmap (this file)
├─ Dockerfile                    # Builds Angular and serves via nginx with key injection
├─ docker/entrypoint-frontend.sh # Renders nginx.conf from env, starts nginx
├─ nginx.conf                    # SPA + /api proxy with header injection
└─ wifiequ-frontend/             # Angular workspace
```

## Implementation Plan
1. **Scaffold & hygiene**  
   - Keep the Angular workspace synced with the current CLI version.  
   - Add ESLint + Prettier config to enforce linting in `npm run lint`.  
   - Establish environment files (`environment.ts`) for base URLs, mock mode, and SSE toggles. *(pending)*
2. **Data layer**  
   - Wrap REST + SSE access in services (done) and add retry/backoff for the stream.  
   - Provide a mock provider returning fixture data so components can run without the daemon. *(pending)*
3. **UI components**  
   - Equalizer visuals with lightweight CSS (done).  
   - Surface daemon metadata (mode/interface/refresh/defined bins) and improve zero-signal states (in progress).  
   - Add onboarding tips for mock mode and hardware requirements. *(pending)*
4. **Testing & quality**  
   - Add Jest/Karma tests for services/components; snapshot the channel bars for regression checks. *(pending)*  
   - Add Cypress/Playwright smoke tests hitting a mocked backend. *(pending)*  
   - Run `npm run test`, `npm run lint`, and `npm run e2e` in CI. *(pending)*
5. **Build & deployment**  
   - Production build served via nginx with stats-key injection (done).  
   - Compose entry pairs with backend; add health checks and metrics. *(pending)*  
   - Document local development flow (`ng serve`, SSE proxy configuration) and cross-origin considerations (in progress).

## Domain Notes
- SSE requires manual teardown in Angular; leverage `EventSource` wrappers with RxJS Observables and handle reconnection/backoff gracefully.  
- Balancing the equalizer: reuse channel labels from the daemon config so tooltips and legends match the CLI demo.  
- Accessibility: focus on high-contrast colours and add ARIA labels for channels so screen readers announce the signal strength.

## Testing Checklist
- `npm run lint` and `npm run test` remain green in mock and live configurations.  
- Contract snapshots compare service outputs to recorded JSON (`/channels`, `/stats`).  
- Visual regression tests capture channel bar layout across viewport sizes.

## Implementation Timeline
- **2025-10-27: workspace scaffold** — Angular README outlined client intentions and linked to the API contract.
- **2025-12-03: live stream & container** — UI renders SSE stream with signal colouring; nginx-based container added with server-side stats-key injection and Compose pairing.

## Initial setup
- Install Angular CLI (global): `npm install -g @angular/cli`
- Scaffold the app inside `wifiequ/web-angular`: `ng new wifiequ-frontend --style=scss --routing`
- Install dev dependencies: `cd wifiequ/web-angular/wifiequ-frontend && npm install`
- Proxy to avoid CORS in dev: `proxy.conf.json`:
  ```json
  {
    "/api": {
      "target": "http://localhost:8080",
      "secure": false,
      "changeOrigin": true
    }
  }
  ```
- Dev server (uses proxy, serves on http://localhost:4200): `ng serve --proxy-config proxy.conf.json`


## Related Documentation
- Module overview: [README.md](README.md)  
- Shared contract: [../api/README.md](../api/README.md)  
- Linux reference implementation: [../linux/README.md](../linux/README.md)  
- Project-wide notes: [../NOTES.md](../NOTES.md)
