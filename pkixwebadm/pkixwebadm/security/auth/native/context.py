from __future__ import annotations

from dataclasses import dataclass
from pkixwebadm import Settings, UserRepository, SessionStore
from typing import Callable

PasswordVerifier = Callable[[str, str], bool]


@dataclass(frozen=True)
class NativeAuthContext:
    """Aggregates dependencies required by the native auth manager."""

    settings: Settings
    users: UserRepository
    # TODO: add a separate persistence object reference
    sessions: SessionStore
    verify_password: PasswordVerifier
