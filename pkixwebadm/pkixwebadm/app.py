from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles
from libdmotservices import get_resource_base_name

from . import (
    PATH_WEB_STATIC,
    PROJECT_NAME,
    URL_STATIC,
    VERSION,
    root,
    Settings,
    get_settings
)

def create_app() -> FastAPI:
    settings = get_settings()
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
    app.include_router(root.router)
    return app
