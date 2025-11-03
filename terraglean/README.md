# terraglean (planned)

Showcase Java service that will expose a small REST API for analysing locations and persisting the results, with a future lightweight UI to visualise the data.

## Current status
- Requirements and stack selection are in planning.
- Implementation will begin after the WiFiEqu enhancements settle.

## Draft architecture
- **Runtime**: Spring Boot 3 (or Micronaut as a fallback) with embedded Tomcat/Jetty.
- **Endpoints**: `GET /api/v1/sites`, `POST /api/v1/analyze`, `GET /api/v1/health`.
- **Persistence**: JPA entities on top of H2/SQLite JDBC for easy demos.
- **Build & tests**: Gradle (preferred) or Maven paired with JUnit 5 integration tests.
- **Packaging**: Docker image exposing port 8080; optional Compose entry to run alongside WiFiEqu.
- **UI exploration** (later): minimal React or Angular single-page view for analysis results.

## Near-term tasks
- Finalise whether Spring Boot or Micronaut offers the lighter footprint for the sample.
- Model entity/repository structure for storing analysed site metadata and reports.
- Sketch integration tests that hit the REST endpoints with in-memory persistence.
- Outline Dockerfile and Compose wiring to share infrastructure with the existing portfolio.

## Related
- Developer notes: [NOTES.md](NOTES.md)
- Back to [portfolio overview](../README.md)
