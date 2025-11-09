# Terraglean – Future Work Notes
Part of the [`cv-samples`](../README.md) portfolio. Terraglean will be a showcase Java service once the WiFiEqu enhancements land.

## Concept
Interactive location-analysis service that ingests world data layers (time, weather, country stats) and exposes a lightweight REST API with a future UI for visualisation.

## Current status
- Requirements and stack selection remain in planning.
- Implementation kicks off after the WiFiEqu roadmap items (Windows worker, Docker orchestration) stabilise.

## Draft architecture
- **Runtime:** Spring Boot 3 (Micronaut remains a fallback) with embedded Tomcat/Jetty.
- **Endpoints:** `GET /api/v1/sites`, `POST /api/v1/analyze`, `GET /api/v1/health`.
- **Persistence:** JPA entities backed by H2/SQLite JDBC for demo-friendly storage.
- **Build & tests:** Gradle (preferred) or Maven with JUnit 5 integration coverage.
- **Packaging:** Docker image exposing port 8080, optional Compose service beside WiFiEqu.
- **UI exploration (later):** React or Angular single-page view to visualise the analysis output.

## Near-term tasks
- Finalise Spring Boot vs. Micronaut based on footprint and DX.
- Model entities/repositories for analysed sites and reports.
- Sketch integration tests hitting the REST endpoints using in-memory persistence.
- Outline Dockerfile/Compose wiring that reuses the portfolio’s shared infrastructure.

## Related documents
- [Future work index](future_work.md)
- [Portfolio overview](../README.md#future-work-index)

