from __future__ import annotations

from passlib.hash import bcrypt

from pkixwebadm import get_settings


def password_hash(password: str | None = None) -> str | None:
    """Hash ``password`` with the configured bcrypt rounds."""

    if password is None:
        return None
    settings = get_settings()
    password_rounds = settings.pass_rounds
    password_hash = bcrypt.using(rounds=password_rounds).hash(password)
    return password_hash


def password_verify(password: str | None, hashed: str | None) -> bool:
    """Return ``True`` when ``password`` matches the stored hash."""

    if not password or not hashed:
        return False
    # note: rehashing the password and comparing would produce a new salt and the function would always fail
    return bcrypt.verify(password, hashed)
