# NOTES

Developer notes for Terraglean — the planned map analytics service. These notes articulate the intended architecture, research tasks, and development plan before implementation begins.

## Implementation Plan
1. **Architecture validation**  
   - Finalise the technology choice (Spring Boot vs. Micronaut) by prototyping REST + persistence scaffolds.  
   - Define domain entities (`Site`, `Layer`, `Observation`) and data access patterns (JPA + repositories).  
   - Sketch integration points for external data sources (weather APIs, time services).
2. **Service foundations**  
   - Scaffold the project with Gradle (`init` + opinionated directory layout).  
   - Add configuration management (profiles for dev/test/prod, YAML config) and central logging.  
   - Implement health probes (`/actuator/health`, `/actuator/info`) and document them.
3. **Core features**  
   - `POST /api/v1/analyze`: ingest a location payload, fetch external layers, compute summary metrics, persist result.  
   - `GET /api/v1/sites`: list stored analyses with pagination and filtering.  
   - `GET /api/v1/sites/{id}`: retrieve detailed report including layer breakdown.  
   - Optional: schedule background refreshes for cached analyses.
4. **Data & caching strategy**  
   - Start with H2 (file-backed) for demos; evaluate SQLite via JDBC if simpler for distribution.  
   - Introduce a caching layer (Caffeine or Redis) when external API usage grows.  
   - Document rate limiting and fallback behaviour when upstream APIs are unavailable.
5. **Frontend exploration**  
   - Prototype a small Leaflet-based UI consuming the REST endpoints.  
   - Provide sample GeoJSON overlays and identify accessibility considerations.  
   - Plan for future Angular/React integration once backend stabilises.
6. **DevOps & packaging**  
   - Create Dockerfile and Compose entry; document environment variables.  
   - Add CI workflow running unit/integration tests and producing container images.  
   - Document deployment scenarios (local demo, cloud container).

## Research Backlog
- Evaluate public datasets/APIs for weather, daylight, and population statistics (licensing, rate limits).  
- Investigate map tile providers compatible with Leaflet (OpenStreetMap, Carto).  
- Decide on geospatial libraries (GeoTools, JTS) and confirm license compatibility.

## Testing Checklist
- Unit tests (JUnit 5) for services, repositories, and controllers.  
- Integration tests using Testcontainers (PostgreSQL or H2) for repository layer.  
- Contract tests for external API integrations with recorded fixtures (WireMock).  
- Performance smoke tests for high-latency API responses.

## Implementation Timeline
- **2025-10-10: Initial skeleton** — Repository created with the high-level aim of a Spring Boot map analytics service.
- **2025-10-27 – 2025-11-02: Planning rounds** — Documentation updates fleshed out the architecture draft and future work backlog while `.gitattributes` alignment kept repo tooling consistent. Implementation remains in the planning stage.

## Related Documentation
- Planning overview: [README.md](README.md)  
- Portfolio context: [../README.md](../README.md)
