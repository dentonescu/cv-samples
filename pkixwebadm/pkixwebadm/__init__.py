#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path

DEFAULT_FILE_ENCODING: str = "utf-8"
PROJECT_NAME: str = "pkixwebadm"
VERSION: str = (Path(__file__).resolve().parent/".version").read_text(encoding=DEFAULT_FILE_ENCODING).strip()

from .config import get_settings
from .cli import main  # noqa: E402

__all__ = [
    "DEFAULT_FILE_ENCODING",
    "PROJECT_NAME",
    "VERSION",
    "get_settings",
    "main"
]
