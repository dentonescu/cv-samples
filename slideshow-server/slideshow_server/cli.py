"""Command-line interface helpers for the slideshow server package.

The functions here create the CLI argument parser and launch the HTTP server,
serving as the glue between user input and the underlying media/server
utilities.
"""

from __future__ import annotations

import argparse
import os
import random
import subprocess
from http.server import HTTPServer
from pathlib import Path
from typing import Sequence

from . import VERSION

from .media import (
    IMAGE_EXT,
    VIDEO_EXT,
    discover_media_files,
    load_assets
)

from .server import (
    create_handler,
    render_index
)

def build_arg_parser() -> argparse.ArgumentParser:
    """Create the CLI argument parser for configuring a slideshow session.

    Returns:
        An ``ArgumentParser`` preloaded with slideshow-specific options.
    """
    parser = argparse.ArgumentParser(description="Random media slideshow server")
    parser.add_argument("media_dir", help="Path to media directory")
    parser.add_argument(
        "--context",
        type=str,
        default="/",
        help="The context of the slideshow (default: /)",
    )
    parser.add_argument(
        "--bind_address",
        type=str,
        default="127.0.0.1",
        help="Bind address (default: 127.0.0.1)",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=8080,
        help="Port number (default: 8080)",
    )
    parser.add_argument(
        "--interval",
        type=int,
        default=5000,
        help="Refresh interval in ms (default: 5000)",
    )
    return parser

#
# slideshow-server entry point
#

def main(argv: Sequence[str] | None = None) -> None:
    """Parse CLI arguments and start the slideshow HTTP server.

    Args:
        argv: Optional sequence of argument strings, used primarily for tests.
              When omitted, ``sys.argv`` is consumed.
    """
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    media_dir = Path(args.media_dir).resolve()
    media_files = discover_media_files(media_dir, IMAGE_EXT, VIDEO_EXT)
    random.shuffle(media_files)

    module_dir = Path(__file__).resolve().parent
    assets_dir = module_dir.parent
    css, html_template, js = load_assets(
        assets_dir / "slideshow-server.css",
        assets_dir / "slideshow-server.html",
        assets_dir / "slideshow-server.js",
    )
    html_document = render_index(media_files, args.interval, css, html_template, js)

    handler_class = create_handler(
        args.context,
        html_document,
        VIDEO_EXT,
        subprocess.Popen,
    )

    os.chdir(media_dir)
    print("=======================================================")
    print(f"slideshow-server version {VERSION}")
    print("=======================================================")
    print(
        f"Serving slideshow from {media_dir} "
        f"at http://{args.bind_address}:{args.port}{args.context}"
    )
    httpd = HTTPServer((args.bind_address, args.port), handler_class)
    try:
        httpd.serve_forever()
    finally:
        httpd.server_close()
