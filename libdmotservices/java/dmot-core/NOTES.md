# NOTES

Developer notes for `libdmotservices-core`. This module is the general-purpose Java utility set shared across portfolio projects.

## Current File Layout
```
dmot-core/
├─ README.md                     # Module overview and usage
├─ NOTES.md                      # Developer roadmap (this file)
├─ build.xml                     # Ant wrapper for Maven goals
├─ pom.xml                       # Maven module descriptor
├─ dep/jars/json-20231013.jar    # Third-party dependency checked in
├─ src/
│  ├─ main/java/dev/dmotservices/api/**     # HTTP/client abstractions
│  ├─ main/java/dev/dmotservices/beans/**   # Sample DTOs
│  ├─ main/java/dev/dmotservices/util/**    # Utility helpers
│  └─ test/java/dev/dmotservices/**         # JUnit test suites
└─ .idea/ (workspace metadata, gitignored in final tree)
```

## Implementation Plan
1. **Library scope**  
   - Keep the API tight: utilities for I/O, regex, URIs, collections, and time. Resist adding heavy frameworks.  
   - Group helpers into cohesive packages (`dev.dmot.core.io`, `dev.dmot.core.time`, etc.) with clear Javadoc.  
   - Provide extension points (interfaces) for features that may later move into other modules.
2. **Quality & coverage**  
   - Expand JUnit tests to cover boundary conditions (e.g., URI normalisation with IPv6, regex helpers with Unicode).  
   - Add property-based tests where useful (QuickTheories) to hammer edge cases.  
   - Integrate static analysis (`spotbugs`, `errorprone`) via Maven profiles.
3. **Documentation**  
   - Maintain `docs/` snippets showing imports and typical usage; integrate with the module README.  
   - Generate Javadoc to `target/apidocs` and publish under `dist/docs/java/core`.  
   - Document breaking changes in a module-level `CHANGELOG.md`.
4. **Distribution**  
   - Provide Maven Central deployment instructions (even if publishing locally).  
   - Produce shaded jar variant only if dependency bundling becomes necessary (default to thin jar).  
   - Ensure module version matches the root `VERSION`.
5. **Integration**  
   - Add sample `examples/` showcasing interoperability with WiFiEqu or future services (e.g., channel data parsing).  
   - Encourage reuse by keeping method signatures consistent across languages (inspired by the C helpers).

## Domain Notes
- Use Java 17 features (records, `java.time`) where they simplify code but keep binary compatibility with `--release 17`.  
- Avoid third-party dependencies; where unavoidable, mark them optional.  
- Provide thread-safe variants for utilities that may be shared across executors.

## Implementation Timeline
- **2025-10-11: Core reboot** — Module carved out of the parent repo with migrated tests and refreshed documentation to reflect the new baseline.
- **2025-10-29: Version helpers** — Semantic version utilities and matching unit tests aligned the Java surface with the C counterparts.
- **2025-11-01 – 2025-11-02: Build tuning** — Follow-up changes polished the build and normalised attributes after the versioning work.

## Testing Checklist
- `mvn test` and `mvn verify` pass locally and in CI.  
- SpotBugs/ErrorProne profiles run clean.  
- Module jar and sources jar land in `dist/` with correct POM metadata.

## Related Documentation
- Module overview: [README.md](README.md)  
- Parent Java notes: [../NOTES.md](../NOTES.md)  
- Project-wide context: [../../NOTES.md](../../NOTES.md)
