#!/usr/bin/env python3
import os
import argparse
import random
import subprocess
from http.server import SimpleHTTPRequestHandler, HTTPServer

parser = argparse.ArgumentParser(description="Random media slideshow server")
parser.add_argument("media_dir", help="Path to media directory")
parser.add_argument(
    "--context",
    type=str,
    default="/",
    help="The context of the slideshow (default: /)")
parser.add_argument(
    "--bind_address",
    type=str,
    default="127.0.0.1",
    help="Bind address (default: 127.0.0.1)")
parser.add_argument("--port", type=int, default=8080,
                    help="Port number (default: 8080)")
parser.add_argument("--interval", type=int, default=5000,
                    help="Refresh interval in ms (default: 5000)")
args = parser.parse_args()


MEDIA_DIR = os.path.abspath(args.media_dir)
BIND_ADDRESS = args.bind_address
PORT = args.port
WEB_CONTEXT = args.context
NORMALIZED_WEB_CONTEXT = WEB_CONTEXT
if not WEB_CONTEXT.endswith("/"):
    NORMALIZED_WEB_CONTEXT += "/"
REFRESH_INTERVAL = args.interval

IMAGE_EXT = (".bmp", ".gif", ".gifv", ".jpe", ".jpeg", ".jpg", ".png", ".webp")
VIDEO_EXT = (".3g2", ".3gp", ".avi", ".flv", ".m4a", ".mkv", ".mov", ".mp4", ".mpg", ".ogg", ".webm", ".wmv")

media_files = []
for root, _, files in os.walk(MEDIA_DIR, followlinks=True):
    for f in files:
        if f.lower().endswith(IMAGE_EXT + VIDEO_EXT):
            rel_path = os.path.relpath(os.path.join(root, f), MEDIA_DIR)
            media_files.append(rel_path)

random.shuffle(media_files)

pyConstants = f"""
const FILES = {media_files};
const INTERVAL = {REFRESH_INTERVAL};
"""

with open("slideshow-server.css", "r", encoding="utf-8") as f:
    css = f.read()
with open("slideshow-server.html", "r", encoding="utf-8") as f:
    html = f.read()
with open("slideshow-server.js", "r", encoding="utf-8") as f:
    js = f.read()
html = html.replace("<!-- CONST_INJECTION_POINT -->", pyConstants)
html = html.replace("<!-- CSS_INJECTION_POINT -->", css)
html = html.replace("<!-- JS_INJECTION_POINT -->", js)


class SlideshowHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        # Respond only to the apropriate web context
        if self.path in {WEB_CONTEXT, NORMALIZED_WEB_CONTEXT, f"{NORMALIZED_WEB_CONTEXT}index.html"}:
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            self.wfile.write(html.encode("utf-8"))
            return

        # Take special action if this is a request for a video
        file_path = self.translate_path(self.path)
        if os.path.isfile(file_path) and file_path.lower().endswith(VIDEO_EXT):
            self.send_response(200)
            self.send_header("Content-Type", "video/mp4")
            self.send_header("Cache-Control", "no-cache")
            self.end_headers()

            # Use ffmpeg to stream
            cmd = [
                "ffmpeg", "-i", file_path, "-f", "mp4",
                "-movflags", "frag_keyframe+empty_moov",
                "-c:v", "libx264", "-preset", "veryfast", "-c:a", "aac",
                "-b:v", "1500k", "-b:a", "128k", "pipe:1"
            ]
            process = subprocess.Popen(cmd, stdout=subprocess.PIPE)
            try:
                while True:
                    chunk = process.stdout.read(8192)
                    if not chunk:
                        break
                    try:
                        self.wfile.write(chunk)
                    except (BrokenPipeError, ConnectionResetError):
                        # client disconnected, so stop ffmpeg
                        process.kill()
                        break
            finally:
                process.stdout.close()
                process.wait()
            return
        else:
            super().do_GET()


##
# Launch the server
##
os.chdir(MEDIA_DIR)
print(f"Serving slideshow from {MEDIA_DIR} at http://{BIND_ADDRESS}:{PORT}{WEB_CONTEXT}")
httpd = HTTPServer((BIND_ADDRESS, PORT), SlideshowHandler)
httpd.serve_forever()
