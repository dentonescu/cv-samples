# Developer notes (backend)

## Current layout
```
backend/
  heapmonj/
    build.gradle
    settings.gradle
    src/
      main/java/dev/dmotservices/heapmonj/
        HeapmonjApplication.java
        dto/HeapStatus.java
      main/resources/application.properties
      test/java/dev/dmotservices/heapmonj/
        PojoTests.java
        HeapmonjApplicationTests.java
```

## Implementation plan & stage
- **Heap snapshot DTO**: `HeapStatus` present with static `now()` snapshot (done).
- **Persistence**: add `HeapSample` JPA entity + `HeapSampleRepository` (pending).
- **Service layer**: `HeapInfoService` to map libdmotservices `HeapInfo` to DTO/entity (pending).
- **REST API**: `/api/v1/heap/current` and `/api/v1/heap/history` with `limit`/`since` (pending).
- **Scheduling**: 10s sampler persisting to H2 (pending).
- **Packaging**: Dockerfile and docker-compose with frontend, CI workflow (pending).

## Build notes
- Requires JDK 21. Gradle toolchain is set to 21; ensure your environment/daemon uses it.
- `./gradlew build` runs libdmotservices version stamping and `mvn -DskipTests install` before compiling.
- Tests may be marked UP-TO-DATE; use `./gradlew test --rerun-tasks` to force execution.
- H2 configured for on-disk storage at `data/` relative to the backend project (gitignored).
