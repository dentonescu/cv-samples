# NOTES

Developer notes for `libdmotservices-servlet-javax`. This module keeps legacy servlet helpers available while the ecosystem moves toward Jakarta APIs.

## Implementation Plan
1. **API shape**  
   - Keep helpers compatible with Servlet 3.1 (`javax.servlet.*`). Avoid accidental use of Jakarta namespaces.  
   - Provide adapters that make it easy to migrate to a future `jakarta.servlet` module (shared interfaces in `dev.dmot.servlet`).  
   - Document thread-safety expectations for filters/listeners provided here.
2. **Testing**  
   - Use embedded containers (Jetty/Tomcat) in integration tests to validate filters and listeners end-to-end.  
   - Include unit tests with Mockito for request/response wrappers.  
   - Mirror tests in a future Jakarta module to guarantee parity.
3. **Packaging**  
   - Produce thin jars (no servlet API inside, `provided` scope).  
   - Publish javadoc + sources jars.  
- Ensure module metadata (`Automatic-Module-Name`) is stable.
4. **Migration path**  
   - Add README appendix noting equivalent classes planned for the Jakarta module.  
   - Provide code examples illustrating how to swap imports and keep behaviour unchanged.  
   - Track breaking changes in a dedicated section of the module changelog.
5. **Observability**  
   - Helpers that interact with logging should default to SLF4J; note how to bridge to JUL or Log4j2.  
   - Document recommended servlet container configuration (async support, filter ordering).

## Domain Notes
- Keep optional dependencies minimal; trust container-provided libs.  
- Focus on portable constructs (avoid vendor-specific APIs).  
- Ensure everything compiles under `--release 17` while targeting legacy runtimes.

## Implementation Timeline
- **2025-10-11: Module extraction** — Legacy servlet helpers split from `dmot-core`, accompanied by build configuration updates and documentation touch-ups.
- **2025-10-29: Version utilities** — Shared version helpers ensured the servlet module follows the same semantic versioning contract as the rest of the project.
- **2025-11-01 – 2025-11-02: Build polish** — Attribute normalisation and documentation updates kept packaging tidy after the versioning work.

## Testing Checklist
- `mvn test` passes with embedded container integration tests.  
- Integration suite spins up Jetty/Tomcat locally via Maven profiles.  
- Dist artifacts include javadoc/sources jars with matching versions.

## Related Documentation
- Module overview: [README.md](README.md)  
- Parent Java notes: [../NOTES.md](../NOTES.md)  
- Project-wide context: [../../NOTES.md](../../NOTES.md)
