# heapmonj backend

Spring Boot (Java 21) service that exposes JVM heap metrics and persists periodic samples.

## Status
- Implemented: DTO `HeapStatus` built from libdmotservices `HeapInfo`, JPA entity/repository `HeapSample`, H2 file-backed storage, scheduler that records snapshots on a fixed rate, and REST endpoints (`/api/v1/heap/current`, `/api/v1/heap/history` with limit/since support). Dockerfile and docker-compose wiring are in place.

## Build & run
```bash
cd backend/heapmonj
./gradlew test        # uses cached results unless --rerun-tasks
./gradlew bootRun
```

Notes:
- JDK 21 required (toolchain + Spring Boot 4).
- `./gradlew build` runs `tools/VersionManager.java` in `libdmotservices` and `mvn -DskipTests install` to publish local jars before compiling.
- H2 stores data on disk at `backend/heapmonj/data/` (gitignored and removed by `make clean`).
- H2 console is disabled by default; connect via JDBC (`jdbc:h2:file:./data/heapdb...`) if you need to inspect locally.

## Roadmap (backend)
- Backend plumbing is in place (sampling, persistence, REST). Future niceties: richer filters on history, optional actuator metrics, and configurable retention.
- Frontend: Angular UI already consumes the endpoints; future polish will focus on UX and charts.
- Packaging: Dockerfiles + compose already working; CI/CD will mirror local `make`/`cmake` targets.
