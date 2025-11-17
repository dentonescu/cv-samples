from __future__ import annotations

try:  # pragma: no cover - compat shim for pydantic v1/v2
    from pydantic import BaseModel, ConfigDict
except ImportError:  # pragma: no cover - pydantic v1 lacks ConfigDict
    from pydantic import BaseModel

    ConfigDict = None


class Credentials(BaseModel):
    """Immutable payload describing how a principal attempts to authenticate."""

    username: str | None = None
    password: str | None = None
    token: str | None = None
    key_file: str | None = None

    if ConfigDict:
        model_config = ConfigDict(frozen=True)
    else:  # pragma: no cover - exercised only under pydantic v1
        class Config:
            frozen = True
