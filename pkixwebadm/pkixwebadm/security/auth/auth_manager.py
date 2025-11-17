"""Contracts shared by all authentication managers."""

from abc import ABC, abstractmethod
from fastapi import Depends
from typing import Any

from pkixwebadm import AUTH_METHOD_NATIVE, Credentials, Identity, Settings, get_settings

# internals
_AUTH_MANAGER_REGISTRY = {AUTH_METHOD_NATIVE: None}


# classes
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


# functions
def get_auth_manager(
    settings: Settings = Depends(get_settings),
) -> AuthManager:
    """Provide an AuthManager based on the configuration."""
    if settings.auth_method == AUTH_METHOD_NATIVE:
        factory = _AUTH_MANAGER_REGISTRY.get(AUTH_METHOD_NATIVE)
        if not callable(factory):
            raise RuntimeError("No auth manager configured.")
        return factory(settings)

    return None


def set_auth_manager(name, factory):
    _AUTH_MANAGER_REGISTRY[name] = factory
