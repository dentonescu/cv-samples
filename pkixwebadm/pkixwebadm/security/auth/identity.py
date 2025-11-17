from __future__ import annotations

from datetime import datetime, timezone
from typing import Tuple

try:  # pragma: no cover - compat shim for pydantic v1/v2
    from pydantic import BaseModel, ConfigDict, Field
except ImportError:  # pragma: no cover - pydantic v1 lacks ConfigDict
    from pydantic import BaseModel, Field

    ConfigDict = None


def _utcnow() -> datetime:
    """Provide timezone-aware timestamps so session math is predictable."""
    return datetime.now(timezone.utc)


class Identity(BaseModel):
    """Immutable representation of an authenticated principal."""

    user_id: str
    username: str
    roles: Tuple[str, ...] = ()
    issued_at: datetime = Field(default_factory=_utcnow)
    display_name: str | None = None
    token: str | None = None

    if ConfigDict:
        model_config = ConfigDict(frozen=True)
    else:  # pragma: no cover - exercised only under pydantic v1
        class Config:
            frozen = True

    def has_role(self, role: str) -> bool:
        """Return True when the identity advertises the requested role."""
        return role in self.roles
