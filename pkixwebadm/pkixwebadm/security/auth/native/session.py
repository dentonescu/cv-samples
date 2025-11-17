from __future__ import annotations
from datetime import datetime, timedelta, timezone
from pkixwebadm import Identity
from threading import Lock
from typing import Dict, Tuple

_SESSION_STORE = None


class SessionStore:
    """Thread-safe session store that keeps identities in memory."""

    def __init__(self) -> None:
        self._sessions: Dict[str, Tuple[Identity, datetime | None]] = (
            {}
        )  # the in-memory session store
        self._lock = (
            Lock()
        )  # ensures atomicity for each operation using "with self._lock"

    def _now(self) -> datetime:
        return datetime.now(timezone.utc)

    def get(self, session_id: str) -> Identity | None:
        if not session_id:
            return None
        with self._lock:
            record = self._sessions.get(session_id)
            if record is None:
                return None
            identity, expires_at = record
            if expires_at and expires_at <= self._now():
                self._sessions.pop(
                    session_id, None
                )  # clear the session if it's expired
                return None
            return identity

    def set(
        self, session_id: str, identity: Identity, ttl_seconds: int | None = None
    ) -> None:
        expires_at = None
        if ttl_seconds and ttl_seconds > 0:
            expires_at = self._now() + timedelta(seconds=ttl_seconds)
        elif ttl_seconds == 0:
            expires_at = self._now()
        with self._lock:
            self._sessions[session_id] = (identity, expires_at)

    def delete(self, session_id: str) -> None:
        if not session_id:
            return
        with self._lock:
            self._sessions.pop(session_id, None)


def get_session_store() -> SessionStore:
    """Return the shared session store, instantiating it on first use."""

    global _SESSION_STORE
    if _SESSION_STORE is None:
        _SESSION_STORE = SessionStore()
    return _SESSION_STORE
