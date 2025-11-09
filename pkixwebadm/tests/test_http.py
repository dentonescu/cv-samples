import logging

from http import HTTPStatus
from fastapi.responses import HTMLResponse
from pkixwebadm import get_error_page

logger = logging.getLogger(__name__)

def test_err_page():
    response: HTMLResponse = get_error_page("This error page was called from the unit test, test_err_page()")
    assert response.status_code == HTTPStatus.BAD_REQUEST

