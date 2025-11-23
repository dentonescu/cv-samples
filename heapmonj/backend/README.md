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

## Roadmap (backend)
- Implement `HeapSample` entity/repository and `HeapInfoService`.
- Add `/api/v1/heap/current` and `/api/v1/heap/history` (with `limit`/`since`) controllers.
- Add 10s scheduler to persist snapshots.
- Wire H2 config (already present) into the Spring data stack.
- Dockerfile and CI integration after endpoints stabilize.
