# heapmonj

A Java heap monitor demo (Spring Boot backend + Angular frontend) for CV samples.

## Status
- Backend scaffolded (Gradle/Spring Boot) with a heap DTO (`HeapStatus`).
- libdmotservices tooling/docs refreshed; dependency ready to consume.
- MVP endpoints, persistence, scheduler, frontend, and Docker/CI wiring are still pending.

## Near-term plan
- Backend (Java 21, Spring Boot): add `HeapSample` JPA entity/repo, `HeapInfoService`, `/api/v1/heap/{current,history}` controller (with `limit`/`since`), scheduler sampling every 10s, and H2 on-disk storage under `backend/heapmonj/data/`.
- Frontend: Angular app with a polling service and Chart.js line chart for heap usage.
- Packaging: Dockerfiles for backend/frontend and docker-compose to wire them together; GitHub Actions to run Gradle tests/build, Angular build, and optionally Docker build.

If you land here now, expect work in progress; the roadmap above outlines what’s coming next.***

## Build notes
- Requires JDK 21 for the backend (Gradle toolchain + Spring Boot 4).
- `./gradlew build` will:
  - Run the libdmotservices version stamper (`java tools/VersionManager.java ../..`).
  - Run `mvn -DskipTests install` in `libdmotservices/java` to publish local jars before compiling.
  - Use cached test results unless you run `./gradlew test --rerun-tasks` (tests may be skipped as UP-TO-DATE otherwise).
- H2 writes to `backend/heapmonj/data/`; database files are gitignored.

## Navigation
- Backend: [backend/](backend/README.md)
- Backend notes: [backend/NOTES.md](backend/NOTES.md)
- Frontend: [frontend/](frontend/README.md)
