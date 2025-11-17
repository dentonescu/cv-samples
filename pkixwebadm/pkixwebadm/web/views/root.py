from fastapi import APIRouter, Depends, Request
from fastapi.templating import Jinja2Templates
from http import HTTPStatus

from pkixwebadm import (
    PAGE_WEB_ROOT,
    PATH_WEB_TEMPLATES,
    PROJECT_NAME,
    URL_ROOT,
    VERSION,
    get_error_page,
    require_current_user,
)

try:
    templates = Jinja2Templates(directory=str(PATH_WEB_TEMPLATES))
except AssertionError:
    templates = None

router = APIRouter()


@router.get(URL_ROOT, name="ROOT")
async def view_root_get(
    request: Request,
    _current_user=Depends(require_current_user), # must be logged-in to access this page
):
    """Render the landing page with project metadata.

    Falls back to the static error page when Jinja templates cannot be loaded.
    """
    context = {
        # template variables for Jinja
        "request": request,
        "project_name": PROJECT_NAME,
        "project_version": VERSION,
    }
    if templates is None:
        return get_error_page(
            "Templating engine unavailable. The landing page could not be displayed.",
            HTTPStatus.INTERNAL_SERVER_ERROR,
        )

    return templates.TemplateResponse(request, PAGE_WEB_ROOT, context)
