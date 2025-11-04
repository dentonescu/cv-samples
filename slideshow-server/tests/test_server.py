import io
import subprocess

from http import HTTPStatus
from http.server import SimpleHTTPRequestHandler

from slideshow_server import (
    HTTP_ERR_FILE_NOT_FOUND,
    SlideshowHandler,
    create_handler,
    normalize_context,
    render_index,
    serve_index_response,
)

def test_render_index_injects_constants_and_assets():
    html_template = "<!-- CONST_INJECTION_POINT --><style><!-- CSS_INJECTION_POINT --></style><script><!-- JS_INJECTION_POINT --></script>"
    css = "body { color: red; }"
    js = "console.log('hi');"
    html = render_index(["img.jpg"], 1234, css, html_template, js)
    assert "const FILES = [\"img.jpg\"];" in html
    assert "const INTERVAL = 1234;" in html
    assert css in html
    assert js in html


def test_serve_index_response_writes_html():
    class DummyHandler:
        def __init__(self):
            self.status = None
            self.headers = []
            self.ended = False
            self.wfile = io.BytesIO()

        def send_response(self, status):
            self.status = status

        def send_header(self, key, value):
            self.headers.append((key, value))

        def end_headers(self):
            self.ended = True

    handler = DummyHandler()
    requested_path = "/slides"
    allowed_context = "/slides"
    allowed_context_normalized = "/slides/"
    served = serve_index_response(handler, requested_path, allowed_context, allowed_context_normalized, "<html>ok</html>")
    assert served is True
    assert handler.status == HTTPStatus.OK
    assert ("Content-type", "text/html") in handler.headers
    assert handler.ended is True
    assert handler.wfile.getvalue() == b"<html>ok</html>"


def test_serve_index_response_rejects_other_paths():
    class DummyHandler:
        def __init__(self):
            self.wfile = io.BytesIO()

        def send_response(self, *_):
            return None

        def send_header(self, *_):
            return None

        def end_headers(self):
            return None

    handler = DummyHandler()

    requested_path = "/other"
    allowed_context = "/slides"
    allowed_context_normalized = "/slides/"
    served = serve_index_response(handler, requested_path, allowed_context, allowed_context_normalized, "<html>ok</html>")
    assert served is False
    assert handler.wfile.getvalue() == b""


def test_create_handler_normalizes_context_and_assigns_html():
    handler_cls = create_handler("/slides", "<html>doc</html>", ['.mp4'], subprocess.Popen)
    assert handler_cls.web_context == "/slides"
    assert handler_cls.normalized_web_context == "/slides/"
    assert handler_cls.html_document == "<html>doc</html>"


def test_handler_serve_video_streams_chunks():
    CHUNK1 = b"chunk1"
    CHUNK2 = b"chunk2"
    chunks = [CHUNK1, CHUNK2, b""]

    class DummyProcess:
        def __init__(self):
            self.stdout = io.BytesIO(b"".join(chunks))
            self.killed = False
            self.waited = False

        def kill(self):
            self.killed = True

        def wait(self):
            self.waited = True

    calls = []

    def fake_factory(command, stdout=None):
        calls.append((command, stdout))
        return DummyProcess()

    class DummyHandler(SlideshowHandler):
        subprocess_factory = staticmethod(fake_factory)

        def __init__(self):
            self.wfile = io.BytesIO()
            self.responses = []
            self.headers = []

        def send_response(self, code):
            self.responses.append(code)

        def send_header(self, key, value):
            self.headers.append((key, value))

        def end_headers(self):
            self.headers.append(("end", "headers"))

    handler = DummyHandler()
    handler.serve_video("/tmp/video.mov")
    assert calls
    assert handler.responses == [HTTPStatus.OK]
    assert ("Content-Type", "video/mp4") in handler.headers
    assert calls[0][1] == subprocess.PIPE
    assert handler.wfile.getvalue() == b"".join([CHUNK1, CHUNK2])


def test_normalize_context_appends_trailing_slash():
    assert normalize_context("/slides") == "/slides/"
    assert normalize_context("/slides/") == "/slides/"


def test_handler_missing_file_triggers_404(monkeypatch):
    class DummyHandler(SlideshowHandler):
        def __init__(self):
            self.web_context = "/"
            self.normalized_web_context = "/"
            self.html_document = ""
            self.video_ext = ()
            self.wfile = io.BytesIO()
            self.path = "/missing"
            self.errors = []

        def translate_path(self, path):
            return "/does/not/exist"

        def send_error(self, code, message=None):
            self.errors.append((code, message))

    def fake_super_do_get(self):
        raise FileNotFoundError

    monkeypatch.setattr(SimpleHTTPRequestHandler, "do_GET", fake_super_do_get, raising=False)
    handler = DummyHandler()
    handler.do_GET()
    assert handler.errors == [(HTTPStatus.NOT_FOUND, HTTP_ERR_FILE_NOT_FOUND)]
