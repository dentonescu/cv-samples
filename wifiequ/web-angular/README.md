# WiFiEqu (Angular)

Angular client that visualises channel data from the WiFiEqu JSON API.

## Status
- Angular UI renders live `/api/v1/channels/stream` data with per-channel bars, fills missing bins with a placeholder level, and colour-codes signal strength.
- Nginx proxy injects the stats API key server-side (`WFQ_STATS_KEY`) so the client never embeds secrets.
- Production build is packaged in a dedicated container (`wifiequ-frontend`); see the root `docker-compose.yml` for ports and pairing with the backend.

## Related
- Back to [WiFiEqu overview](../README.md)
- API notes: [wifiequ/api](../api/README.md)
- Developer notes: [NOTES.md](NOTES.md)
