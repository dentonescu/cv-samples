import os
from pathlib import Path
from typing import Sequence

"""Utilities for locating media assets and preparing static resources."""

IMAGE_EXT = (".bmp", ".gif", ".gifv", ".jpe", ".jpeg", ".jpg", ".png", ".webp")
VIDEO_EXT = (".3g2", ".3gp", ".avi", ".flv", ".m4a", ".mkv", ".mov", ".mp4", ".mpg", ".ogg", ".webm", ".wmv")

def discover_media_files(
    media_dir: Path,
    image_ext: Sequence[str] = IMAGE_EXT,
    video_ext: Sequence[str] = VIDEO_EXT,
) -> list[str]:
    """Collect relative media file paths within ``media_dir``.

    Args:
        media_dir: Root directory to scan.
        image_ext: Iterable of allowable image extensions.
        video_ext: Iterable of allowable video extensions.

    Returns:
        Relative paths (as strings) for media files matching the whitelisted
        extensions. Paths that escape ``media_dir`` via symlinks are skipped.
    """
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

def build_ffmpeg_command(file_path: str) -> list[str]:
    """Produce the ffmpeg invocation for re-encoding ``file_path`` on demand.

    Args:
        file_path: Path to the source media file to re-encode.

    Returns:
        A list of command arguments suitable for ``subprocess.Popen``.
    """
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

def load_assets(css_path: Path, html_path: Path, js_path: Path) -> tuple[str, str, str]:
    """Load the static asset files required by the slideshow UI.

    Args:
        css_path: Filesystem path to the stylesheet.
        html_path: Filesystem path to the HTML template.
        js_path: Filesystem path to the JavaScript bundle.

    Returns:
        A tuple of ``(css, html, js)`` strings, each decoded as UTF-8.
    """
    css = css_path.read_text(encoding="utf-8")
    html = html_path.read_text(encoding="utf-8")
    js = js_path.read_text(encoding="utf-8")
    return css, html, js
