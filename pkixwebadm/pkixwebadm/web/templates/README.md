# Templates

Jinja templates rendered by pkixwebadm.

- `base.html` supplies the HTML skeleton, Bootstrap includes, and the global logout form.
- `ROOT.html` extends `base.html` to show project metadata for authenticated visitors.
- `login.html` extends the base layout with the sign-in form used by `/auth/login`.
- `widgets/` holds reusable partials such as the certificate-ingestion panel.

Additional templates (dashboard, certificate detail pages, error states) will live alongside these. Return to the [web README](../README.md) for navigation.
