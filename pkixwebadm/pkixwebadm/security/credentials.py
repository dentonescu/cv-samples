from __future__ import annotations

from pydantic import BaseModel


class Credentials(BaseModel):
    """Immutable payload describing how a principal attempts to authenticate."""

    username: str | None = None
    password: str | None = None
    token: str | None = None
    key_file: str | None = None

    class Config:
        frozen = True