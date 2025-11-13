import logging
import pytest
import types

from fastapi import Response
from pkixwebadm import (
    AuthenticationError,
    Credentials,
    Identity,
    NativeAuthManager,
    NativeAuthContext,
    NativeUserRepository,
    get_settings,
)

logger = logging.getLogger(__name__)

#
# Constants
#
BAD_PASSWORD = "NOT_THE_REAL_PASSWORD"
BOOTSTRAP_ADMIN_USER_ID = "u007"
BOOTSTRAP_ADMIN_USER_NAME = "admin"
PKIXWA_SETTINGS = get_settings()
SESSION_IDENTIFIER_BOOTSTRAP_ADMIN = (
    "some-identification-string-which-corresponds-to-the-session-of-the-logged-in-user"
)

#
# Helpers
#


class MockSessions:
    entries: dict[str, Identity]

    def __init__(self, entries):
        self.entries = entries

    def get(self, session_id):
        return self.entries.get(session_id)

    def set(self, *args, **kwargs):
        pass

    def delete(self, session_id):
        self.entries.pop(session_id, None)


def _generate_request(cookie_name, value):
    req = types.SimpleNamespace()
    req.cookies = {cookie_name: value}
    return req


def _generate_context_without_sessions():
    ctx = NativeAuthContext(
        settings=PKIXWA_SETTINGS,
        users=NativeUserRepository(),
        sessions=MockSessions({}),
        verify_password=_mock_password_verify,
    )
    return ctx


def _generate_context_with_logged_in_admin_session():
    identity = Identity(
        user_id=BOOTSTRAP_ADMIN_USER_ID, username=BOOTSTRAP_ADMIN_USER_NAME
    )
    ctx = NativeAuthContext(
        settings=PKIXWA_SETTINGS,
        users=NativeUserRepository(),
        sessions=MockSessions({SESSION_IDENTIFIER_BOOTSTRAP_ADMIN: identity}),
        verify_password=lambda *args, **kwargs: True,  # on-the-fly method which signals the password's always correct
    )
    return ctx


def _mock_password_verify(password: str | None, hashed: str | None) -> bool:
    if not password or not hashed:
        status = False
    elif hashed.startswith("$") and len(hashed) >= 30:
        status = True  # just assume the hash is correct for the given password
    if password == BAD_PASSWORD:
        status = False
    logger.debug(
        "_mock_password_verify: password=%s, hashed=%s, status=%s",
        password,
        hashed,
        status,
    )
    return status


#
# Unit tests
#


def test_authenticate_bootstrap_admin_returns_identity():
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    credentials = Credentials(
        username=PKIXWA_SETTINGS.bootstrap_admin_user,
        password=PKIXWA_SETTINGS.bootstrap_admin_pass_hash,
    )
    identity: Identity = manager.authenticate(credentials)
    assert identity.username == PKIXWA_SETTINGS.bootstrap_admin_user


def test_authenticate_bootstrap_admin_raises_authentication_error():
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    credentials = Credentials(
        username=PKIXWA_SETTINGS.bootstrap_admin_user,
        password=BAD_PASSWORD,
    )
    with pytest.raises(AuthenticationError):
        identity = manager.authenticate(credentials)


def test_is_logged_in_returns_true():
    ctx = _generate_context_with_logged_in_admin_session()
    manager = NativeAuthManager(ctx)
    req = _generate_request(
        PKIXWA_SETTINGS.session_cookie_name, SESSION_IDENTIFIER_BOOTSTRAP_ADMIN
    )
    assert manager.is_logged_in(req) is True


def test_is_logged_in_returns_false():
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    req = _generate_request(
        PKIXWA_SETTINGS.session_cookie_name,
        "some-session-identifier-which-isnt-in-the-session-store",
    )
    assert manager.is_logged_in(req) is False


def test_gets_current_user():
    ctx = _generate_context_with_logged_in_admin_session()
    manager = NativeAuthManager(ctx)
    req = _generate_request(
        PKIXWA_SETTINGS.session_cookie_name, SESSION_IDENTIFIER_BOOTSTRAP_ADMIN
    )
    identity = manager.get_current_user(req)
    assert identity.user_id == BOOTSTRAP_ADMIN_USER_ID


def test_logs_user_out_correctly():
    ctx = _generate_context_with_logged_in_admin_session()
    manager = NativeAuthManager(ctx)
    req = _generate_request(
        PKIXWA_SETTINGS.session_cookie_name, SESSION_IDENTIFIER_BOOTSTRAP_ADMIN
    )
    assert manager.is_logged_in(req) is True
    resp = Response()
    manager.log_out(req, resp)
    assert manager.is_logged_in(req) is False
