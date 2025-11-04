#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path

"""Public package surface for the slideshow server application.

This module wires together the CLI entry point, media discovery helpers, and
HTTP server utilities so callers can import a cohesive API via
``import slideshow_server``.
"""
VERSION = Path("VERSION").read_text(encoding="utf-8").strip()

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
    "HTTP_ERR_FILE_NOT_FOUND",
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
