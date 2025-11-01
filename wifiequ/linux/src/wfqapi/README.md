# WiFiEqu (Linux) — src/wfqapi/

HTTP façade for the Linux daemon.

- `router.c` – maps REST paths to handlers.
- `http.c` – wraps libmicrohttpd and manages the sample ring buffer.
- `json.c` – serialises samples and configuration payloads.
- [`endpoints/`](endpoints/README.md) – concrete request handlers, including the SSE feed.

Back to [`../README.md`](../README.md).
