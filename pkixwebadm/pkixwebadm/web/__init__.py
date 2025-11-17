from __future__ import annotations

from http import HTTPStatus
from fastapi import Depends, HTTPException, Request, status
from fastapi.responses import HTMLResponse
from pathlib import Path
from textwrap import dedent
from libdmotservices import DEFAULT_FILE_ENCODING

from urllib.parse import quote

from pkixwebadm import (
    PROJECT_NAME,
    VERSION,
    AuthManager,
    Identity,
    get_auth_manager,
)

#
# Externals
#

# Page names
PAGE_WEB_ERROR = "error.html"
PAGE_WEB_LOGIN = "login.html"
PAGE_WEB_ROOT = "ROOT.html"

# File system locations
PATH_WEB_ROOT = Path(__file__).resolve().parent
PATH_WEB_TEMPLATES = PATH_WEB_ROOT / "templates"
PATH_WEB_STATIC = PATH_WEB_ROOT / "static"
PATH_WEB_STATIC_HTML = PATH_WEB_STATIC / "html"
PATH_WEB_STATIC_HTML_ERROR_PAGE = PATH_WEB_STATIC_HTML / PAGE_WEB_ERROR

# Endpoints
ENDPOINT_LOGIN = "/login"
ENDPOINT_LOGOUT = "/logout"

# URLs
URL_ROOT = "/"
URL_AUTH = "/auth"
URL_LOGIN = URL_AUTH + ENDPOINT_LOGIN
URL_STATIC = "/static"

# Query parameters
QUERY_PARAM_NEXT = "next"

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


async def require_current_user(
    request: Request,
    auth_manager: AuthManager = Depends(get_auth_manager),
) -> Identity:
    """Ensure the request is associated with a logged-in user."""

    identity = auth_manager.get_current_user(request)
    if identity is not None:
        return identity

    next_path = request.url.path
    if request.url.query:
        next_path = f"{next_path}?{request.url.query}"
    location = URL_LOGIN
    if next_path and not next_path.startswith(URL_AUTH):
        encoded_next = quote(next_path, safe="/?=&")
        location = f"{location}?{QUERY_PARAM_NEXT}={encoded_next}"
    raise HTTPException(
        status_code=status.HTTP_303_SEE_OTHER,
        headers={"Location": location},
    )
