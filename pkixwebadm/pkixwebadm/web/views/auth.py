from __future__ import annotations

import logging

from http import HTTPStatus
from typing import Any

from fastapi import APIRouter, Depends, Request, Response
from fastapi.responses import RedirectResponse
from fastapi.security import OAuth2PasswordRequestForm
from fastapi.templating import Jinja2Templates

from pkixwebadm import (
    AuthenticationError,
    Credentials,
    ENDPOINT_LOGIN,
    ENDPOINT_LOGOUT,
    PAGE_WEB_LOGIN,
    PATH_WEB_TEMPLATES,
    PROJECT_NAME,
    QUERY_PARAM_NEXT,
    URL_AUTH,
    URL_LOGIN,
    URL_ROOT,
    get_auth_manager,
    get_error_page,
)

logger = logging.getLogger(__name__)

#
# Internals
#


def _sanitize_next_param(next_url: str | None) -> str | None:
    if not next_url:
        return None
    if next_url.startswith(("http://", "https://", "//")):
        return None
    if not next_url.startswith("/"):
        return None
    return next_url


#
# Externals
#

try:
    templates = Jinja2Templates(directory=str(PATH_WEB_TEMPLATES))
except AssertionError:
    templates = None

router = APIRouter(prefix=URL_AUTH, tags=["auth"])


# Login form rendering
@router.get(ENDPOINT_LOGIN, name="login")
async def view_login_get(request: Request) -> Any:
    """Render the login page."""

    if templates is None:
        return get_error_page(
            "Templating engine unavailable. The login form could not be displayed.",
            HTTPStatus.INTERNAL_SERVER_ERROR,
        )

    next_url = _sanitize_next_param(request.query_params.get(QUERY_PARAM_NEXT))
    context = {
        # template variables for Jinja
        "request": request,
        "project_name": PROJECT_NAME,
        "next_url": next_url,
        "show_logout": False,
    }
    return templates.TemplateResponse(PAGE_WEB_LOGIN, context)


# Login form submission and processing
@router.post(ENDPOINT_LOGIN)
async def view_login_post(
    request: Request,
    response: Response,
    form_data: OAuth2PasswordRequestForm = Depends(),
    auth_manager=Depends(get_auth_manager),
) -> Any:
    """Process login submissions and issue sessions."""

    credentials = Credentials(username=form_data.username, password=form_data.password)
    logger.debug(
        "Credentials parsed. form_data.username=%s, credentials.username=%s",
        form_data.username,
        credentials.username,
    )
    try:
        identity = auth_manager.authenticate(credentials)
        logger.debug("Identity retrieved for username: %s", credentials.username)
    except AuthenticationError:
        if templates is None:
            return get_error_page(
                "Templating engine, status_code unavailable. The login submission could not be processed.",
                HTTPStatus.INTERNAL_SERVER_ERROR,
            )
        context = {
            "request": request,
            "project_name": PROJECT_NAME,
            "error": "Invalid username or password.",
            "next_url": _sanitize_next_param(
                request.query_params.get(QUERY_PARAM_NEXT)
            ),
            "show_logout": False,
        }
        return templates.TemplateResponse(
            PAGE_WEB_LOGIN, context, status_code=HTTPStatus.UNAUTHORIZED
        )
    redirect_target = (
        _sanitize_next_param(request.query_params.get(QUERY_PARAM_NEXT)) or URL_ROOT
    )
    redirect_response = RedirectResponse(
        url=redirect_target, status_code=HTTPStatus.SEE_OTHER
    )
    auth_manager.issue_session(redirect_response, identity)
    return redirect_response


# Logout function -- POST because it causes a state change and we don't want a GET triggering it accidentally
@router.post(ENDPOINT_LOGOUT, name="logout")
async def view_logout_post(
    request: Request,
    response: Response,
    auth_manager=Depends(get_auth_manager),
) -> Any:
    """Tear down the session cookie and redirect to the login page."""

    auth_manager.log_out(request, response)
    return RedirectResponse(url=URL_LOGIN, status_code=HTTPStatus.SEE_OTHER)
