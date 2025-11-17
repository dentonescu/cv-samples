# Views

FastAPI routers that render HTML pages for pkixwebadm.

- `root.py` serves the landing page and provides the templating fallback by calling `get_error_page`. It now requires an authenticated session (redirects happen in the dependency layer).
- `auth.py` hosts the login/logout routes and uses the native auth manager to mint cookie-backed sessions.

Future routers (certificate dashboards, API endpoints) will live alongside this module. Return to the [web README](../README.md) for navigation.
