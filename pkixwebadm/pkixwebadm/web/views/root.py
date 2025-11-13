from fastapi import APIRouter, Depends, Request
from fastapi.templating import Jinja2Templates
from http import HTTPStatus

from pkixwebadm import (
    PAGE_WEB_ROOT,
    PATH_WEB_TEMPLATES,
    PROJECT_NAME,
    VERSION,
    Settings,
    get_error_page,
    get_settings,
)

try:
    templates = Jinja2Templates(directory=str(PATH_WEB_TEMPLATES))
except AssertionError:
    templates = None

router = APIRouter()


@router.get("/", name="ROOT")
async def view_root(
    request: Request,
    settings: Settings = Depends(get_settings),
):
    """Render the landing page with project metadata.

    Falls back to the static error page when Jinja templates cannot be loaded.
    """
    context = {
        "request": request,
        "project_name": PROJECT_NAME,
        "project_version": VERSION,
    }
    if templates is None:
        return get_error_page(
            "Templating engine unavailable; rendered via inline HTML.",
            HTTPStatus.INTERNAL_SERVER_ERROR,
        )

    return templates.TemplateResponse(request, PAGE_WEB_ROOT, context)
