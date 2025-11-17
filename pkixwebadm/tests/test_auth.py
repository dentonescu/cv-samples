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
    SessionStore,
    get_auth_manager,
    get_settings,
)

logger = logging.getLogger(__name__)


@pytest.fixture
def anyio_backend():
    return "asyncio"


#
# Constants
#

BAD_PASSWORD = "this-is-not-the-actual-password-so-it-will-fail"
SIMULATED_PASSWORD = "THIS_IS_A_SIMULATED_PASSWORD__DEFINITElY_NOT_A_REAL_LIFE_PASSWORD"
BOOTSTRAP_ADMIN_USER_ID = "u007"
BOOTSTRAP_ADMIN_USER_NAME = "admin"
PKIXWA_SETTINGS = get_settings()
SESSION_ID_BOOTSTRAP_ADMIN = (
    "some-identification-string-which-corresponds-to-the-session-of-the-logged-in-user"
)
SESSION_ID_TEST = "some-session-id"

#
# Helpers
#


class _DummyJSONRequest:
    """Async-friendly request stub that returns a JSON payload."""

    def __init__(self, payload: dict[str, str]):
        self._payload = payload

    async def json(self):
        return self._payload

    async def form(self):
        raise RuntimeError("form parsing not supported in this stub")

    async def body(self):
        from json import dumps

        return dumps(self._payload).encode("utf-8")


class _MockSessions:
    entries: dict[str, Identity]

    def __init__(self, entries=None):
        self.entries = entries or {}
        self.saved_ttls: dict[str, int | None] = {}

    def get(self, session_id):
        return self.entries.get(session_id)

    def set(self, session_id, identity, ttl_seconds=None):
        self.entries[session_id] = identity
        self.saved_ttls[session_id] = ttl_seconds

    def delete(self, session_id):
        self.entries.pop(session_id, None)


def _generate_request(session_id):
    req = types.SimpleNamespace()
    cookie_name = PKIXWA_SETTINGS.session_cookie_name
    req.cookies = {cookie_name: session_id}
    return req


def _generate_context_without_sessions():
    ctx = NativeAuthContext(
        settings=PKIXWA_SETTINGS,
        users=NativeUserRepository(),
        sessions=_MockSessions({}),
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
        sessions=_MockSessions({SESSION_ID_BOOTSTRAP_ADMIN: identity}),
        verify_password=lambda *args, **kwargs: True,  # on-the-fly method which signals the password's always correct
    )
    return ctx


def _mock_password_verify(password: str | None, hashed: str | None) -> bool:
    status = False
    if password and hashed and hashed.startswith("$") and len(hashed) >= 30:
        status = True  # just assume the hash is correct for the given password
    if password != SIMULATED_PASSWORD:
        status = False
    logger.debug(
        "_mock_password_verify: password=%s, hashed=%s, status=%s",
        password,
        hashed,
        status,
    )
    return status


async def _login_test_helper(user, pass_hash):
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    payload = {
        "username": user,
        "password": pass_hash,
    }
    request = _DummyJSONRequest(payload)
    response = Response()
    logged_in_status = await manager.login(request, response)
    return logged_in_status, ctx, response


#
# Unit tests
#


def test_authenticate_bootstrap_admin_returns_identity():
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    credentials = Credentials(
        username=PKIXWA_SETTINGS.bootstrap_admin_user, password=SIMULATED_PASSWORD
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
    req = _generate_request(SESSION_ID_BOOTSTRAP_ADMIN)
    assert manager.is_logged_in(req) is True


def test_is_logged_in_returns_false():
    ctx = _generate_context_without_sessions()
    manager = NativeAuthManager(ctx)
    req = _generate_request("some-session-identifier-which-isnt-in-the-session-store")
    assert manager.is_logged_in(req) is False


def test_gets_current_user():
    ctx = _generate_context_with_logged_in_admin_session()
    manager = NativeAuthManager(ctx)
    req = _generate_request(SESSION_ID_BOOTSTRAP_ADMIN)
    identity = manager.get_current_user(req)
    assert identity.user_id == BOOTSTRAP_ADMIN_USER_ID


@pytest.mark.anyio("asyncio")
async def test_login_with_json_body_issues_session(monkeypatch):
    logged_in_status, ctx, response = await _login_test_helper(
        PKIXWA_SETTINGS.bootstrap_admin_user, SIMULATED_PASSWORD
    )
    assert logged_in_status is True
    assert ctx.sessions.entries  # session stored
    # FastAPI/Starlette populates the Set-Cookie header which contains a mapping-like object
    assert PKIXWA_SETTINGS.session_cookie_name in response.headers.get("set-cookie", "")


@pytest.mark.anyio("asyncio")
async def test_login_fails_when_bad_credentials_supplied(monkeypatch):
    logged_in_status, ctx, response = await _login_test_helper(
        PKIXWA_SETTINGS.bootstrap_admin_user, BAD_PASSWORD
    )
    assert logged_in_status is False


def test_logs_user_out_correctly():
    ctx = _generate_context_with_logged_in_admin_session()
    manager = NativeAuthManager(ctx)
    req = _generate_request(SESSION_ID_BOOTSTRAP_ADMIN)
    assert manager.is_logged_in(req) is True
    resp = Response()
    manager.log_out(req, resp)
    assert manager.is_logged_in(req) is False


def test_session_store_honours_set_and_delete():
    store = SessionStore()
    identity = Identity(user_id="2512", username="santa_claus")
    store.set(SESSION_ID_TEST, identity)
    assert store.get(SESSION_ID_TEST) == identity
    store.delete(SESSION_ID_TEST)
    assert store.get(SESSION_ID_TEST) is None


def test_session_store_honours_ttl():
    store = SessionStore()
    identity = Identity(user_id="001", username="hokage")
    store.set(SESSION_ID_TEST, identity, ttl_seconds=0)
    assert store.get(SESSION_ID_TEST) is None


def test_get_auth_manager():
    auth_manager = get_auth_manager(PKIXWA_SETTINGS)
    assert auth_manager is not None
    kind = type(auth_manager).__name__
    logger.debug("auth_manager kind = %s", kind)
    assert kind in ("NativeAuthManager")  # expand this list when more are added
