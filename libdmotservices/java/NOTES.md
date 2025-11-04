# NOTES

Developer notes for the Java side of libdmotservices. This file captures the shared strategy for the Maven/Ant workspace and the submodules (`dmot-core`, `dmot-servlet-javax`).

## Current File Layout
```
java/
├─ README.md                     # Java track overview
├─ NOTES.md                      # Developer roadmap (this file)
├─ build.xml / build.sh / build.cmd
├─ log4j2.xml                    # Logging configuration for samples/tests
├─ mock/                         # Sample apps and test doubles
├─ dmot-core/
│  ├─ README.md / NOTES.md
│  ├─ build.xml
│  ├─ pom.xml
│  ├─ dep/jars/json-20231013.jar
│  └─ src/
│     ├─ main/java/dev/dmotservices/**   # Core APIs and utilities
│     └─ test/java/dev/dmotservices/**   # Unit tests
└─ dmot-servlet-javax/
   ├─ README.md / NOTES.md
   ├─ build.xml
   └─ src/
      ├─ main/java/dev/dmotservices/servlet/**  # Servlet adapters
      └─ test/java/dev/dmotservices/servlet/**  # Unit tests
```

## Implementation Plan
1. **Build orchestration**  
   - Keep Ant (`build.xml`) responsible for cross-module tasks (version stamping, artifact aggregation) while Maven handles per-module builds.  
   - Ensure `gen-version.py` feeds `${revision}` into both Ant and Maven during CI.  
   - Add Gradle build scripts once the modules stabilise so consumers can pick their preferred tooling.
2. **Module evolution**  
   - `dmot-core`: continue curating general-purpose utilities; document API stability guarantees and deprecation policies.  
   - `dmot-servlet-javax`: isolate legacy helpers, add Jakarta equivalents later in a sibling module.  
   - Keep README + NOTES for each module updated as APIs evolve.
3. **Testing discipline**  
   - JUnit 5 for unit tests, with parameterised cases covering edge inputs.  
   - Introduce Testcontainers-based integration tests once persistence/network helpers appear.  
   - Surface test coverage reports (`mvn verify -Pcoverage`) and publish summary badges if valuable.
4. **Publishing & packaging**  
   - Provide scripts for `mvn deploy` to publish to a local Maven repo (`dist/m2`).  
   - Collect shaded jars (if needed) and plain jars in `dist/` with checksums.  
   - Document semantic version bump rules (sync with root `VERSION`).
5. **Documentation & samples**  
   - Generate Javadoc (`mvn javadoc:javadoc`) and host under `docs/java/`.  
   - Provide small usage snippets in `examples/` or `mock/` demonstrating each utility.  
   - Keep API examples consistent with consumers (Terraglean, planned Spring Boot services).

## Domain Notes
- Java baseline is 17; everything compiles with `--release 17` for portability.  
- Logging defaults to SLF4J-friendly interfaces; avoid binding to a specific logger in the libraries.  
- `mock/` directory houses sample applications; ensure they stay in sync with library updates to aid manual testing.

## Implementation Timeline
- **2021-10 – 2021-12: Original Maven roots**  
  Early work created the Maven module, adjusted groupIds, and experimented with Jakarta namespaces before the project paused.
- **2025-10-11: Modernisation sprint**  
  Java 17 migration, module splits, and the Ant parent build laid the groundwork for today’s structure. Tests moved to conventional locations, and refreshed documentation/screenshots reset the presentation.
- **2025-10-29 – 2025-11-02: Versioning parity**  
  Cross-language semantic version utilities and follow-up build corrections kept the Java modules aligned with the C side and producing consistent artifacts.

## Testing Checklist
- `ant` (or `./build.sh`) runs cleanly, invoking Maven builds/tests under the hood.  
- Individual modules succeed with `mvn test` and `mvn verify`.  
- Dist artifacts carry the correct version metadata in `META-INF/MANIFEST.MF`.

## Related Documentation
- Module overview: [README.md](README.md)  
- Core utilities: [dmot-core/README.md](dmot-core/README.md)  
- Servlet helpers: [dmot-servlet-javax/README.md](dmot-servlet-javax/README.md)  
- Project-wide notes: [../NOTES.md](../NOTES.md)
