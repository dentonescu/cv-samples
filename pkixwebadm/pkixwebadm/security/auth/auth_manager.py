"""Contracts shared by all authentication managers."""

from abc import ABC, abstractmethod
from typing import Any

from pkixwebadm import Credentials, Identity

SESSION_ID_LEN = 32


class AuthenticationError(RuntimeError):
    """Raised when authentication fails."""


class AuthManager(ABC):
    """Minimal API each authentication backend must implement."""

    @abstractmethod
    def authenticate(self, credentials: Credentials) -> Identity:
        """Validate the supplied credentials and return an :class:`Identity`."""

    @abstractmethod
    def is_logged_in(self, request: Any) -> bool:
        """Return ``True`` when the request carries a valid session."""

    @abstractmethod
    def get_current_user(self, request: Any) -> Identity | None:
        """Resolve the authenticated user for the request, if any."""

    @abstractmethod
    def log_out(self, request: Any, response: Any) -> None:
        """Tear down the active session and clear cookies/headers."""
