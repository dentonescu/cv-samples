from __future__ import annotations

from dataclasses import dataclass
from typing import Callable

from .user import UserRepository
from .session import SessionStore
from pkixwebadm import Settings

PasswordVerifier = Callable[[str, str], bool]


@dataclass(frozen=True)
class NativeAuthContext:
    """Aggregates dependencies required by the native auth manager."""

    settings: Settings
    users: UserRepository
    # TODO: add a separate persistence object reference
    sessions: SessionStore
    verify_password: PasswordVerifier
