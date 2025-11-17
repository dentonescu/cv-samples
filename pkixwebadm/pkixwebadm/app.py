"""Application factory and static-asset wiring for pkixwebadm."""

import logging

from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from libdmotservices import get_resource_base_name
from pkixwebadm import (
    PATH_WEB_STATIC,
    PROJECT_NAME,
    URL_STATIC,
    VERSION,
    configure_logging,
    get_settings,
)
from .web.views import auth, root


def create_app() -> FastAPI:
    """Instantiate and configure the FastAPI application."""

    settings = get_settings()
    configure_logging(logging.DEBUG if settings.dev_mode else logging.INFO)
    app = FastAPI(
        title=PROJECT_NAME,
        version=VERSION or "0.0.0",
        debug=settings.dev_mode,
    )
    app.mount(
        URL_STATIC,
        StaticFiles(directory=str(PATH_WEB_STATIC)),
        name=get_resource_base_name(str(PATH_WEB_STATIC)),
    )
    app.include_router(auth.router)
    app.include_router(root.router)
    return app
