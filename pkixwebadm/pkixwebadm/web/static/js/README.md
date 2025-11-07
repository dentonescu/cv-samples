# pkixwebadm Web JS

Client-side helpers that back the certificate ingestion widget.

## Files
- `app.js` — bootstraps the `window.pkixwa` namespace, exposes shared constants, and wires the vendored libdmotservices helpers.
- `certificate_ingest--dragndrop.js` — drag-and-drop upload handling (uses `pkixwa.attachFilesToFormData`).
- `certificate_ingest--fetch_url.js` — URL submission logic that POSTs to `/api/v1/certificates/url`.

## Development
Run `make lint-js` (or `npm run lint:js`) from the project root to lint these scripts. Tests live in `libdmotservices/js/tests` because the shared helpers are bundled upstream.
