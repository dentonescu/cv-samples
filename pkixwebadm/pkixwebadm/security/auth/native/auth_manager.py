from __future__ import annotations

import json
import logging
import secrets
from fastapi import Depends
from typing import Any, Mapping
from urllib.parse import parse_qs

from ..auth_manager import AuthenticationError, AuthManager, set_auth_manager
from .context import NativeAuthContext
from .crypto import password_verify
from .user import NativeUserRepository
from .session import get_session_store

from pkixwebadm import (
    AUTH_METHOD_NATIVE,
    DEFAULT_FILE_ENCODING,
    REQUEST_ATTR_PASSWORD,
    REQUEST_ATTR_USERNAME,
    SESSION_ID_LEN,
    Credentials,
    Identity,
    Settings,
    get_settings,
)

logger = logging.getLogger(__name__)

# error messages
ERR_CREDENTIALS_REQUIRED = "Username and password are required."
ERR_INVALID_CREDENTIALS = "Invalid username or password."

# attributes
COOKIE_ATTR_SAMESITE = "lax"
FASTAPI_ATTR_COOKIES = "cookies"
FASTAPI_ATTR_SET_COOKIE = "set_cookie"
FASTAPI_ATTR_DELETE_COOKIE = "delete_cookie"

TTL_SECONDS_DEFAULT = 86400  # one day


# functions
def get_native_auth_manager(
    settings: Settings = Depends(get_settings),
) -> NativeAuthManager:
    """Provide a NativeAuthManager wired with the shared session store."""
    context = NativeAuthContext(
        settings=settings,
        users=NativeUserRepository(),
        sessions=get_session_store(),
        verify_password=password_verify,
    )
    return NativeAuthManager(context)


# classes
class NativeAuthManager(AuthManager):
    """Credential validator + session helper for the native auth backend."""

    def __init__(self, context: NativeAuthContext):
        self._ctx = context

    #
    # Internals
    #

    @property
    def _cookie_name(self) -> str:
        return self._ctx.settings.session_cookie_name  # env(PKIXWA_SESSION_COOKIE_NAME)

    def _extract_session_id(self, request: Any) -> str | None:
        """Return the session cookie value if it exists on the request."""

        cookies = getattr(request, FASTAPI_ATTR_COOKIES, None)
        if isinstance(cookies, Mapping):
            value = cookies.get(self._cookie_name)
            if value:
                return value
        return None

    def _set_cookie(self, response: Any, session_id: str) -> None:
        """Persist the session identifier on the response."""

        setter = getattr(response, FASTAPI_ATTR_SET_COOKIE, None)
        if callable(setter):
            setter(
                key=self._cookie_name,
                value=session_id,
                httponly=True,  # disallow document.cookie access
                secure=not self._ctx.settings.dev_mode,
                samesite=COOKIE_ATTR_SAMESITE,
                max_age=self._ctx.settings.session_ttl_seconds,
            )
        else:
            raise RuntimeError("Response object does not support set_cookie.")

    def _clear_cookie(self, response: Any) -> None:
        """Remove the session cookie from the response."""

        deleter = getattr(response, FASTAPI_ATTR_DELETE_COOKIE, None)
        if callable(deleter):
            deleter(self._cookie_name)
        else:
            raise RuntimeError("Response object does not support delete_cookie.")

    async def _extract_credentials_from_request(
        self, request: Any
    ) -> Credentials | None:
        """Best-effort extraction of username/password pairs from the request body."""

        # Developer's note
        # ================
        # Potential problems with this function:
        # - JSON parsing still happens first, so non-JSON form payloads fall back to a lenient parser.

        payload: dict[str, Any] | None = None
        body_reader = getattr(request, "body", None)
        raw_body: bytes | None = None
        if callable(body_reader):
            try:
                raw_body = await body_reader()
            except Exception as exc:  # pragma: no cover - defensive logging
                logger.debug("Failed to read request body: %s", exc)
        if raw_body:
            try:
                payload = json.loads(raw_body.decode(DEFAULT_FILE_ENCODING))
            except (ValueError, UnicodeDecodeError):
                # could be application/x-www-form-urlencoded. e.g. in the POST body, username=someguy&password=somepassword
                parsed = parse_qs(
                    raw_body.decode(DEFAULT_FILE_ENCODING, errors="ignore")
                )
                if parsed:
                    payload = {
                        key: values[0] for key, values in parsed.items() if values
                    }
        if not payload:
            return None
        username = payload.get(REQUEST_ATTR_USERNAME)
        password = payload.get(REQUEST_ATTR_PASSWORD)
        if not username or not password:
            return None
        return Credentials(username=username, password=password)

    #
    # Core auth_manager contract implementation
    #
    def authenticate(self, credentials: Credentials) -> Identity:
        """Validate username/password pairs and return the stored identity."""

        username = credentials.username or ""
        password = credentials.password or ""
        if not username or not password:
            raise AuthenticationError(ERR_CREDENTIALS_REQUIRED)
        record = self._ctx.users.get_by_username(username)
        if record is None:
            raise AuthenticationError(ERR_INVALID_CREDENTIALS)
        if not self._ctx.verify_password(password, record.verification_token):
            raise AuthenticationError(ERR_INVALID_CREDENTIALS)
        return record.identity

    def is_logged_in(self, request: Any) -> bool:
        """Return ``True`` if the request already maps to a session."""

        return self.get_current_user(request) is not None

    def get_current_user(self, request: Any) -> Identity | None:
        """Resolve the stored identity for the request session."""

        session_id = self._extract_session_id(request)
        if not session_id:
            return None
        return self._ctx.sessions.get(session_id)

    def log_out(self, request: Any, response: Any) -> None:
        """Delete the session (if present) and clear the cookie."""

        session_id = self._extract_session_id(request)
        if session_id:
            self._ctx.sessions.delete(session_id)
        self._clear_cookie(response)

    #
    # Externals
    #

    @property
    def context(self) -> NativeAuthContext:
        """Expose the context so tests can assert on dependencies."""

        return self._ctx

    def issue_session(
        self, response: Any, identity: Identity, ttl_seconds: int | None = None
    ) -> str:
        """Create a new session id, persist it, and set the cookie."""

        session_ttl_seconds = (
            ttl_seconds
            if ttl_seconds is not None
            else self._ctx.settings.session_ttl_seconds or TTL_SECONDS_DEFAULT
        )
        while True:
            # make sure that the returned random value doesn't already exist in the session store
            session_id = secrets.token_urlsafe(SESSION_ID_LEN)
            if self._ctx.sessions.get(session_id):
                continue
            self._ctx.sessions.set(
                session_id,
                identity,
                ttl_seconds=session_ttl_seconds,
            )
            break
        self._set_cookie(response, session_id)
        return session_id

    async def login(
        self, request: Any, response: Any, ttl_seconds: int | None = None
    ) -> bool:
        """Best-effort FastAPI helper that parses credentials, authenticates, and issues a session."""

        # Developer's note
        # ================
        # Potential problems with this function:
        # - Tied to async request parsing; has to run inside an event loop with a Starlette/FastAPI-style request object
        # - CLI code and synchronous tests can't touch this and have to fall back to authenticate() + issue_session()
        # - Represents a second overlapping way to log in which makes it harder to keep things consistent.

        credentials = await self._extract_credentials_from_request(request)
        if credentials is None:
            logger.debug("Login aborted: request did not contain username/password.")
            return False
        try:
            identity = self.authenticate(credentials)
        except AuthenticationError:
            logger.debug(
                "Authentication failed for user %s.",
                credentials.username or "<unknown>",
            )
            return False
        session_id = self.issue_session(
            response,
            identity,
            ttl_seconds=ttl_seconds,
        )
        if not session_id:
            logger.debug(
                "Failed to issue a session for user %s.",
                credentials.username or "<unknown>",
            )
            return False
        logger.debug("Session %s issued for user %s.", session_id, credentials.username)
        return True


# register this auth manager
set_auth_manager(AUTH_METHOD_NATIVE, get_native_auth_manager)
