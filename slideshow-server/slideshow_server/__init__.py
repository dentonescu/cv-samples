#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import os
import random
import subprocess
from http.server import HTTPServer, SimpleHTTPRequestHandler
from pathlib import Path
from typing import Callable, Iterable, Sequence

"""Core slideshow server components shared by the CLI and unit tests."""

HTTP_ERR_FILE_NOT_FOUND = "File not found"
IMAGE_EXT = (".bmp", ".gif", ".gifv", ".jpe", ".jpeg", ".jpg", ".png", ".webp")
VIDEO_EXT = (".3g2", ".3gp", ".avi", ".flv", ".m4a", ".mkv", ".mov", ".mp4", ".mpg", ".ogg", ".webm", ".wmv")
VERSION = Path("VERSION").read_text(encoding="utf-8").strip()

def build_arg_parser() -> argparse.ArgumentParser:
    """Create and return the CLI argument parser used by the slideshow command."""
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


def normalize_context(web_context: str) -> str:
    """Ensure the configured web context always includes a trailing slash."""
    if web_context.endswith("/"):
        return web_context
    return f"{web_context}/"


def discover_media_files(
    media_dir: Path,
    image_ext: Sequence[str] = IMAGE_EXT,
    video_ext: Sequence[str] = VIDEO_EXT,
) -> list[str]:
    """Return relative media file paths beneath ``media_dir`` that match allowed extensions."""
    media_dir = Path(media_dir).resolve()
    allowed_ext = tuple(e.lower() for e in (*image_ext, *video_ext))

    media_files: list[str] = []
    for root, _, files in os.walk(media_dir, followlinks=True):
        for filename in files:
            if not filename.lower().endswith(allowed_ext):
                continue
            path = Path(root, filename)
            try:
                rel_path = path.relative_to(media_dir)
            except ValueError:
                # Skip files that escape the media directory via symlinks.
                continue
            media_files.append(str(rel_path))
    return media_files


def load_assets(css_path: Path, html_path: Path, js_path: Path) -> tuple[str, str, str]:
    """Read and return the CSS, HTML, and JavaScript asset contents."""
    css = css_path.read_text(encoding="utf-8")
    html = html_path.read_text(encoding="utf-8")
    js = js_path.read_text(encoding="utf-8")
    return css, html, js


def render_index(
    media_files: Iterable[str],
    interval: int,
    css: str,
    html_template: str,
    js: str,
) -> str:
    """Inject configuration constants and static assets into the HTML template."""
    constants = (
        "const FILES = "
        f"{json.dumps(list(media_files))};\n"
        f"const INTERVAL = {interval};\n"
    )
    html = html_template.replace("<!-- CONST_INJECTION_POINT -->", constants)
    html = html.replace("<!-- CSS_INJECTION_POINT -->", css)
    html = html.replace("<!-- JS_INJECTION_POINT -->", js)
    return html


def serve_index_response(
    handler: SimpleHTTPRequestHandler,
    path: str,
    web_context: str,
    normalized_web_context: str,
    html_document: str,
) -> bool:
    """Write the prepared index page to the response if the path targets the slideshow root."""
    allowed_paths = {
        web_context,
        normalized_web_context,
        f"{normalized_web_context}index.html",
    }
    if path not in allowed_paths:
        return False

    handler.send_response(200)
    handler.send_header("Content-type", "text/html")
    handler.end_headers()
    handler.wfile.write(html_document.encode("utf-8"))
    return True


def build_ffmpeg_command(file_path: str) -> list[str]:
    """Produce the ffmpeg command used to re-encode video streams on demand."""
    return [
        "ffmpeg",
        "-i",
        file_path,
        "-f",
        "mp4",
        "-movflags",
        "frag_keyframe+empty_moov",
        "-c:v",
        "libx264",
        "-preset",
        "veryfast",
        "-c:a",
        "aac",
        "-b:v",
        "1500k",
        "-b:a",
        "128k",
        "pipe:1",
    ]


class SlideshowHandler(SimpleHTTPRequestHandler):
    """HTTP handler that serves the slideshow index and streams videos via ffmpeg."""
    web_context: str = "/"
    normalized_web_context: str = "/"
    html_document: str = ""
    video_ext: Sequence[str] = VIDEO_EXT
    subprocess_factory: Callable[..., subprocess.Popen] = staticmethod(subprocess.Popen)

    def do_GET(self) -> None:
        """Serve the slideshow index, stream video content, or defer to the base handler."""

        # display the main page if we're at application root
        if serve_index_response(
            self,
            self.path,
            self.web_context,
            self.normalized_web_context,
            self.html_document,
        ):
            return

        # stream a video if this is a video URL
        file_path = self.translate_path(self.path)
        if os.path.isfile(file_path) and file_path.lower().endswith(tuple(self.video_ext)):
            self.serve_video(file_path)
            return

        # let the built-in HTTP server handle other media, but degrade gracefully when missing
        try:
            super().do_GET()
        except FileNotFoundError:
            self.send_error(404, HTTP_ERR_FILE_NOT_FOUND)

    def serve_video(self, file_path: str) -> None:
        """Stream ``file_path`` to the client by piping ffmpeg output to the response."""
        self.send_response(200)
        self.send_header("Content-Type", "video/mp4")
        self.send_header("Cache-Control", "no-cache")
        self.end_headers()

        process = self.subprocess_factory(
            build_ffmpeg_command(file_path),
            stdout=subprocess.PIPE,
        )

        try:
            if process.stdout is None:
                return

            while True:
                chunk = process.stdout.read(8192)
                if not chunk:
                    break
                try:
                    self.wfile.write(chunk)
                except (BrokenPipeError, ConnectionResetError):
                    process.kill()
                    break
        finally:
            if process.stdout is not None:
                process.stdout.close()
            process.wait()


def create_handler(
    web_context: str,
    html_document: str,
    video_ext: Sequence[str] = VIDEO_EXT,
    subprocess_factory: Callable[..., subprocess.Popen] = subprocess.Popen,
) -> type[SlideshowHandler]:
    """Return a handler subclass pre-configured with slideshow metadata and dependencies."""

    normalized = normalize_context(web_context)
    attrs = {
        "web_context": web_context,
        "normalized_web_context": normalized,
        "html_document": html_document,
        "video_ext": tuple(video_ext),
        "subprocess_factory": staticmethod(subprocess_factory),
    }
    return type("ConfiguredSlideshowHandler", (SlideshowHandler,), attrs)


def main(argv: Sequence[str] | None = None) -> None:
    """CLI entry point that builds the slideshow page and starts the HTTP server."""
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


__all__ = [
    "HTTP_ERR_FILE_NOT_FOUND",
    "IMAGE_EXT",
    "VIDEO_EXT",
    "SlideshowHandler",
    "build_arg_parser",
    "build_ffmpeg_command",
    "create_handler",
    "discover_media_files",
    "load_assets",
    "main",
    "normalize_context",
    "render_index",
    "serve_index_response",
]
