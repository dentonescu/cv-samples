#!/usr/bin/env python3
from __future__ import annotations
from libdmotservices import DEFAULT_FILE_ENCODING
from pathlib import Path

PROJECT_NAME: str = "pkixwebadm"
VERSION: str = (Path(__file__).resolve().parent/".version").read_text(encoding=DEFAULT_FILE_ENCODING).strip()

from .config import (
    Settings,
    get_settings
)

from .logging import (
    configure_logging
)

from pkixwebadm.web import (
    PAGE_WEB_ERROR,
    PAGE_WEB_ROOT,
    PATH_WEB_ROOT,
    PATH_WEB_TEMPLATES,
    PATH_WEB_STATIC,
    PATH_WEB_STATIC_HTML,
    PATH_WEB_STATIC_HTML_ERROR_PAGE,
    URL_STATIC,
    get_error_page
)

from pkixwebadm.web.views import root

from .app import create_app
from .cli import main  # noqa: E402

__all__ = [
    "PAGE_WEB_ERROR",
    "PAGE_WEB_ROOT",
    "PATH_WEB_ROOT",
    "PATH_WEB_TEMPLATES",
    "PATH_WEB_STATIC",
    "PATH_WEB_STATIC_HTML",
    "PATH_WEB_STATIC_HTML_ERROR_PAGE",
    "PROJECT_NAME",
    "VERSION",
    "URL_STATIC",
    "Settings",
    "configure_logging",
    "create_app",
    "get_error_page",
    "get_settings",
    "main",
    "root"
]