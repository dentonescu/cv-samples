# heapmonj backend

Spring Boot (Java 21) service that exposes JVM heap metrics and persists periodic samples.

## Status
- Project scaffolded; DTO `HeapStatus` captures heap snapshots via libdmotservices `HeapInfo`.
- Persistence (H2), sampling scheduler, REST endpoints, and Docker are still to be implemented.

## Build & run
```bash
cd backend/heapmonj
./gradlew test        # uses cached results unless --rerun-tasks
./gradlew bootRun     # after backend endpoints are in place
```

Notes:
- JDK 21 required (toolchain + Spring Boot 4).
- `./gradlew build` runs `tools/VersionManager.java` in `libdmotservices` and `mvn -DskipTests install` to publish local jars before compiling.
- H2 stores data on disk at `backend/heapmonj/data/` (gitignored).
- H2 console is disabled by default; connect via JDBC (`jdbc:h2:file:./data/heapdb...`) if you need to inspect locally.

## Roadmap (backend)
- Backend plumbing is in place (sampling, persistence, REST). Refine history paging/queries as needed.
- Frontend: Angular app with a polling service and Chart.js line chart for heap usage.
- Packaging: Dockerfiles for backend/frontend and docker-compose to wire them together; GitHub Actions to run Gradle tests/build, Angular build, and optionally Docker build.
