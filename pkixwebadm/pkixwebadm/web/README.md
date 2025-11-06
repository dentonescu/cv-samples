# pkixwebadm.web

Presentation layer for pkixwebadm.

## Layout

| Path | Purpose |
|------|---------|
| `views/root.py` | FastAPI router that serves the landing page. |
| `templates/base.html` | Base Jinja layout that other pages will extend. |
| `templates/ROOT.html` | Current landing page; displays project metadata. |
| `static/css/app.css` | Minimal styling for the landing page until Bootstrap returns. |
| `static/html/error.html` | Fallback HTML served when templating is unavailable. |
| `__init__.py` | Shared constants and `get_error_page` helper. |

Extend this module with additional routers (`api/`), templates, and assets as new slices land. Return to the [package README](../README.md) or the implementation plan in [../NOTES.md](../NOTES.md) for broader context.
