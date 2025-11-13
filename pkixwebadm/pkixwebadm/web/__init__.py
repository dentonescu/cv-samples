from __future__ import annotations

from http import HTTPStatus
from fastapi.responses import HTMLResponse
from pathlib import Path
from textwrap import dedent
from libdmotservices import DEFAULT_FILE_ENCODING

from pkixwebadm import PROJECT_NAME, VERSION

# Page names
PAGE_WEB_ERROR = "error.html"
PAGE_WEB_ROOT = "ROOT.html"

# File system locations
PATH_WEB_ROOT = Path(__file__).resolve().parent
PATH_WEB_TEMPLATES = PATH_WEB_ROOT / "templates"
PATH_WEB_STATIC = PATH_WEB_ROOT / "static"
PATH_WEB_STATIC_HTML = PATH_WEB_STATIC / "html"
PATH_WEB_STATIC_HTML_ERROR_PAGE = PATH_WEB_STATIC_HTML / PAGE_WEB_ERROR

# URLs
URL_STATIC = "/static"

# Functions


def get_error_page(
    msg: str, status_code: int | HTTPStatus = HTTPStatus.BAD_REQUEST
) -> HTMLResponse:
    error_msg = msg if msg is not None else ""
    with open(
        PATH_WEB_STATIC_HTML_ERROR_PAGE, "r", encoding=DEFAULT_FILE_ENCODING
    ) as fh:
        error_page = fh.read()
    html = dedent(error_page)
    html = html.replace("{PROJECT_NAME}", PROJECT_NAME)
    html = html.replace("{VERSION}", VERSION)
    html = html.replace("{ERROR_MESSAGE}", error_msg)
    response = HTMLResponse(html)
    response.status_code = int(status_code)
    return response
