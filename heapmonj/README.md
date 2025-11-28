# heapmonj

A Java heap monitor demo (Spring Boot backend + Angular frontend) for CV samples.

## Status
- Backend and frontend MVP are in place: scheduled heap sampling to on-disk H2, REST endpoints (`/api/v1/heap/current` and `/history`), Angular UI with polling + Chart.js, and Docker/compose wiring.
- libdmotservices dependency is stamped/installed before builds (requires Maven + Java 21).

## Build notes
- Requires JDK 21 for the backend (Gradle toolchain + Spring Boot 4).
- `./gradlew build` will:
  - Run the libdmotservices version stamper (`java tools/VersionManager.java ../..`).
  - Run `mvn -DskipTests install` in `libdmotservices/java` to publish local jars before compiling.
  - Use cached test results unless you run `./gradlew test --rerun-tasks` (tests may be skipped as UP-TO-DATE otherwise).
- H2 writes to `backend/heapmonj/data/`; database files are gitignored and removed by `make clean`.

## Navigation
- Backend: [backend/](backend/README.md)
- Backend notes: [backend/NOTES.md](backend/NOTES.md)
- Frontend: [frontend/](frontend/README.md)
- API spec: [api/openapi.yaml](api/openapi.yaml)
- API docs (generate): `make -C heapmonj docs` → outputs to `heapmonj/docs/api/index.html`
  - Requires Node/npx and `@redocly/cli` (installed on demand by the Makefile) for HTML generation.

## Run locally
- Backend: `cd backend/heapmonj && ./gradlew bootRun` (Java 21, Maven available for the libdmotservices hook).
- Frontend (dev): `cd frontend/heapmonj-frontend && npm install && ng serve --proxy-config proxy.conf.json` (serves on 4200, proxies `/api` to `http://localhost:8080`).
- Docker Compose: `docker compose up --build` (landing page on 8080, slideshow 8081, WiFiEqu 8082, heapmonj backend 8083, frontend 8084).
