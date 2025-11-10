import json
import os
import subprocess
from .media import (
    build_ffmpeg_command,
    VIDEO_EXT
)
from http import HTTPStatus
from http.server import SimpleHTTPRequestHandler
from typing import Callable, Iterable, Sequence
from slideshow_server import DEFAULT_FILE_ENCODING

"""HTTP handlers and helpers for serving the slideshow application."""

HTTP_ERR_FILE_NOT_FOUND = "File not found"

def normalize_context(web_context: str) -> str:
    """Ensure the configured web context always includes a trailing slash.

    Args:
        web_context: Configured context path supplied by the user.

    Returns:
        The context path guaranteed to end with ``/``.
    """
    if web_context.endswith("/"):
        return web_context
    return f"{web_context}/"


def render_index(
    media_files: Iterable[str],
    interval: int,
    css: str,
    html_template: str,
    js: str,
) -> str:
    """Inject configuration constants and static assets into the HTML template.

    Args:
        media_files: Iterable of relative media paths.
        interval: Refresh interval in milliseconds for the client.
        css: Raw stylesheet text.
        html_template: HTML template containing placeholder injection markers.
        js: Raw JavaScript bundle text.

    Returns:
        Fully rendered HTML document ready to be served to clients.
    """
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
    """Serve the slideshow index document when the request targets the root.

    Args:
        handler: Active HTTP handler instance.
        path: Parsed request path.
        web_context: Configured web context (possibly lacking trailing slash).
        normalized_web_context: Context guaranteed to end with ``/``.
        html_document: Fully rendered HTML document to write.

    Returns:
        True if the index was served, False if the caller should continue with
        default request handling.
    """
    allowed_paths = {
        web_context,
        normalized_web_context,
        f"{normalized_web_context}index.html",
    }
    if path not in allowed_paths:
        return False

    handler.send_response(HTTPStatus.OK)
    handler.send_header("Content-type", "text/html")
    handler.end_headers()
    handler.wfile.write(html_document.encode(DEFAULT_FILE_ENCODING))
    return True

class SlideshowHandler(SimpleHTTPRequestHandler):
    """HTTP handler that serves the slideshow UI and streams videos via ffmpeg.

    The handler relies on class attributes populated by ``create_handler`` to
    inject the pre-rendered HTML document, web context, and subprocess factory.
    """
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
            self.send_error(HTTPStatus.NOT_FOUND, HTTP_ERR_FILE_NOT_FOUND)

    def serve_video(self, file_path: str) -> None:
        """Stream ``file_path`` to the client through an ``ffmpeg`` subprocess.

        The response is written chunk by chunk, and broken pipe/connection reset
        errors terminate the subprocess cleanly.
        """
        self.send_response(HTTPStatus.OK)
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
    """Return a handler subclass configured with slideshow metadata and dependencies.

    Args:
        web_context: Root path at which the slideshow is exposed.
        html_document: Pre-rendered HTML document served for the index.
        video_ext: Iterable of video file extensions to stream through ffmpeg.
        subprocess_factory: Callable used to spawn ``ffmpeg`` processes; useful
            for injecting mocks during testing.

    Returns:
        A new ``SlideshowHandler`` subclass that carries the provided
        configuration as class attributes.
    """

    normalized = normalize_context(web_context)
    attrs = {
        "web_context": web_context,
        "normalized_web_context": normalized,
        "html_document": html_document,
        "video_ext": tuple(video_ext),
        "subprocess_factory": staticmethod(subprocess_factory),
    }
    return type("ConfiguredSlideshowHandler", (SlideshowHandler,), attrs)
