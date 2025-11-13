from pkixwebadm import Identity
from typing import Protocol


class SessionStore(Protocol):
    """Persistence primitive for session identifiers."""

    def get(self, session_id: str) -> Identity | None: ...

    def set(
        self, session_id: str, identity: Identity, ttl_seconds: int | None = None
    ) -> None: ...

    def delete(self, session_id: str) -> None: ...
