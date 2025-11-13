from abc import ABC, abstractmethod
from pkixwebadm import Identity
from typing import Protocol


class UserRecord(Protocol):
    """Minimal fields required to authenticate a stored user."""

    identity: Identity
    verification_token: str  # password hash, access token, etc.


class UserRepository(ABC):
    """Narrow contract for fetching stored user records."""

    @abstractmethod
    def get_by_username(self, username: str) -> UserRecord | None:
        """Gets a user record according to the user name supplied."""
