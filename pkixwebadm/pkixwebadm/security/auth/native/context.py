from __future__ import annotations

from dataclasses import dataclass
from typing import Callable, Protocol

from pkixwebadm import Identity, Settings


class UserRecord(Protocol):
    """Minimal fields required to authenticate a stored user."""

    identity: Identity
    password_hash: str


class UserRepository(Protocol):
    """Narrow contract for fetching stored user records."""

    def get_by_username(self, username: str) -> UserRecord | None:
        ...


class SessionStore(Protocol):
    """Persistence primitive for session identifiers."""

    def get(self, session_id: str) -> Identity | None:
        ...

    def set(self, session_id: str, identity: Identity, ttl_seconds: int | None = None) -> None:
        ...

    def delete(self, session_id: str) -> None:
        ...


PasswordVerifier = Callable[[str, str], bool]


@dataclass(frozen=True)
class NativeAuthContext:
    """Aggregates dependencies required by the native auth manager."""

    settings: Settings
    users: UserRepository
    sessions: SessionStore
    verify_password: PasswordVerifier
