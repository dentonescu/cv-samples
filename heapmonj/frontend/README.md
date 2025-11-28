# heapmonj frontend

Angular UI to poll the backend and chart JVM heap usage with Chart.js.

## Status
- Implemented: Angular 17 app with a service that polls `/api/v1/heap/current` and `/api/v1/heap/history`, displays last updated time, and renders a Chart.js line chart of heap usage. Uses the Angular proxy to talk to the backend during dev and ships with an nginx Docker image for production.

## Dev server
- Backend on port 8080 (`./gradlew bootRun` in `backend/heapmonj`).
- Frontend on 4200 with the proxy to avoid CORS:
  ```bash
  cd frontend/heapmonj-frontend
  npm install
  ng serve --proxy-config proxy.conf.json
  ```
  The proxy forwards `/api` to `http://localhost:8080` while the app stays on 4200.

## Build
- Production build: `npm run build` → outputs to `dist/heapmonj-frontend/browser` (served by nginx in Docker).
- Docker: `docker compose up --build` publishes the frontend on host port 8084 (nginx inside listens on 80).

## Notes
- Polling interval and history window are in `src/app/config.ts`.
- OpenAPI spec lives at `../api/openapi.yaml` (HTML docs via `make -C .. docs`, requires `@redocly/cli` through npx).
