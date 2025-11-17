# Template Widgets

Reusable Jinja snippets that power the pkixwebadm landing page.

- `certificate_ingest.html` – renders the drag-and-drop uploader plus the URL-fetch form. The associated JavaScript lives under `web/static/js/`.

New widgets should live in this directory so templates can `{% include %}` them consistently.
