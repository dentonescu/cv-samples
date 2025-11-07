# Static assets

Static files served by the FastAPI app.

- `css/` — stylesheets for the Jinja templates.
- `html/` — standalone error pages used when templating fails.
- `js/` — browser scripts powering the certificate ingestion widget.
- `vendor/` — vendored bundles (currently libdmotservices JS helpers).

Mounts are configured in `pkixwebadm.app.create_app`. Return to the [web README](../README.md) for more detail.
