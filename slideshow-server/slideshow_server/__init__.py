#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path

"""Public package surface for the slideshow server application.

This module wires together the CLI entry point, media discovery helpers, and
HTTP server utilities so callers can import a cohesive API via
``import slideshow_server``.
"""
DEFAULT_FILE_ENCODING: str = "utf-8"
PROJECT_NAME: str = "slideshow-server"
VERSION: str = (Path(__file__).resolve().parent/".version").read_text(encoding=DEFAULT_FILE_ENCODING).strip()

from .cli import main  # noqa: E402

from .media import (
    build_ffmpeg_command,
    discover_media_files,
    load_assets
)
from .server import (
    HTTP_ERR_FILE_NOT_FOUND,
    SlideshowHandler,
    create_handler,
    normalize_context,
    render_index,
    serve_index_response
)

__all__ = [
    "DEFAULT_FILE_ENCODING",
    "HTTP_ERR_FILE_NOT_FOUND",
    "PROJECT_NAME",
    "VERSION",
    "SlideshowHandler",
    "build_ffmpeg_command",
    "create_handler",
    "discover_media_files",
    "load_assets",
    "main",
    "normalize_context",
    "render_index",
    "serve_index_response"
]
