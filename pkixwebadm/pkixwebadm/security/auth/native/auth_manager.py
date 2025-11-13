from __future__ import annotations

import secrets
from typing import Any, Mapping

from pkixwebadm import (
    SESSION_ID_LEN,
    AuthenticationError,
    AuthManager,
    Credentials,
    Identity,
)

from .context import NativeAuthContext

# error messages
ERR_CREDENTIALS_REQUIRED = "Username and password are required."
ERR_INVALID_CREDENTIALS = "Invalid username or password."

# attributes
COOKIE_ATTR_SAMESITE = "lax"
FASTAPI_ATTR_COOKIES = "cookies"
FASTAPI_ATTR_SET_COOKIE = "set_cookie"
FASTAPI_ATTR_DELETE_COOKIE = "delete_cookie"


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

    #
    # Externals
    #

    @property
    def context(self) -> NativeAuthContext:
        """Expose the context so tests can assert on dependencies."""

        return self._ctx

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

    def issue_session(self, identity: Identity, response: Any) -> str:
        """Create a new session id, persist it, and set the cookie."""

        session_id = secrets.token_urlsafe(SESSION_ID_LEN)
        self._ctx.sessions.set(
            session_id,
            identity,
            ttl_seconds=self._ctx.settings.session_ttl_seconds,
        )
        self._set_cookie(response, session_id)
        return session_id

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
