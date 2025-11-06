import logging
import sys

from http import HTTPStatus
from fastapi.responses import HTMLResponse
from fastapi.testclient import TestClient
from pkixwebadm import create_app, get_error_page

logger = logging.getLogger(__name__)

def test_err_page():
    response: HTMLResponse = get_error_page("This error page was called from the unit test, test_err_page()")
    assert response.status_code == HTTPStatus.BAD_REQUEST

def test_root_page_renders_project_metadata():
    with TestClient(create_app()) as client:
        response = client.get("/")
        assert response.status_code == 200
        html = response.text
        logger.debug("html=%s", html)
        assert "pkixwebadm" in html
        assert "Version" in html
