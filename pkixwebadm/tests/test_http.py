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

