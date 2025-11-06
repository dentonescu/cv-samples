#!/usr/bin/env python3
from __future__ import annotations
from pathlib import Path

DEFAULT_FILE_ENCODING: str = 'utf-8'

VERSION: str = (Path(__file__).resolve().parent/".version").read_text(encoding=DEFAULT_FILE_ENCODING).strip()

from .args import print_help_all
from .string import get_resource_base_name

__all__ = [
    "get_resource_base_name",
    "print_help_all"
]
