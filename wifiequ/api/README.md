# WiFiEqu (API)

Early notes for the JSON API served by the daemon.

## Endpoints (draft)
- `GET /api/v1/channels` → per‑channel aggregates
- `GET /api/v1/stats` → scan metadata

Schemas will be published here once stabilised.

## Status
- JSON logs from the daemon are available today; schema formalisation is next.
- SSE endpoint and contract tests are tracked in the [WiFiEqu roadmap](../README.md).

## Related
- Back to [WiFiEqu overview](../README.md)
- Implementation details live in the [Linux daemon docs](../linux/README.md)
