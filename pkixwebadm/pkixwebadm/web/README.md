# pkixwebadm.web

Presentation layer for pkixwebadm.

## Layout

| Path | Purpose |
|------|---------|
| `views/root.py` | FastAPI router that serves the landing page. |
| `templates/base.html` | Base Jinja layout that other pages will extend. |
| `templates/ROOT.html` | Current landing page; displays project metadata. |
| `templates/widgets/` | Jinja partials for reusable UI components (e.g., certificate ingestion). |
| `static/css/app.css` | Custom palette layered on top of Bootstrap for the landing page. |
| `static/js/` | Browser logic (see `static/js/README.md`) for the ingestion widget. |
| `static/html/error.html` | Fallback HTML served when templating is unavailable. |
| `static/vendor/libdmotservices/index.js` | Vendored JS helpers (logger, FormData utils) from libdmotservices. |
| `__init__.py` | Shared constants and `get_error_page` helper. |

Extend this module with additional routers (`api/`), templates, and assets as new slices land. Return to the [package README](../README.md) or the implementation plan in [../NOTES.md](../NOTES.md) for broader context.
