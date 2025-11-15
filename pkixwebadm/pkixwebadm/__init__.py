#!/usr/bin/env python3
from __future__ import annotations
from libdmotservices import DEFAULT_FILE_ENCODING
from pathlib import Path

PROJECT_NAME: str = "pkixwebadm"
VERSION: str = (
    (Path(__file__).resolve().parent / ".version")
    .read_text(encoding=DEFAULT_FILE_ENCODING)
    .strip()
)

from .config import Settings, get_settings
from .logging import configure_logging
from .security.credentials import Credentials
from .security.identity import Identity
from .security.auth.auth_manager import SESSION_ID_LEN, AuthenticationError, AuthManager
from .security.auth.user import UserRecord, UserRepository
from .security.auth.session import SessionStore
from .security.auth.native.context import NativeAuthContext
from .security.auth.constants import REQUEST_ATTR_USERNAME, REQUEST_ATTR_PASSWORD
from .security.auth.native.auth_manager import NativeAuthManager
from .security.auth.native.crypto import password_hash, password_verify
from .security.auth.native.user import (
    NativeUserRepository,
    build_bootstrap_admin_record,
)

from .web import (
    PAGE_WEB_ERROR,
    PAGE_WEB_ROOT,
    PATH_WEB_ROOT,
    PATH_WEB_TEMPLATES,
    PATH_WEB_STATIC,
    PATH_WEB_STATIC_HTML,
    PATH_WEB_STATIC_HTML_ERROR_PAGE,
    URL_STATIC,
    get_error_page,
)

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
    "REQUEST_ATTR_USERNAME",
    "REQUEST_ATTR_PASSWORD",
    "SESSION_ID_LEN",
    "URL_STATIC",
    "VERSION",
    "AuthenticationError",
    "AuthManager",
    "Credentials",
    "Identity",
    "NativeAuthContext",
    "NativeAuthManager",
    "NativeUserRepository",
    "Settings",
    "UserRecord",
    "UserRepository",
    "SessionStore",
    "build_bootstrap_admin_record",
    "configure_logging",
    "create_app",
    "get_error_page",
    "get_settings",
    "main",
    "password_hash",
    "password_verify",
]
