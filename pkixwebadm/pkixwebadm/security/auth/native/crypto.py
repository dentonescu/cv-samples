from __future__ import annotations

import bcrypt

from pkixwebadm import DEFAULT_FILE_ENCODING, get_settings


def password_hash(password: str | None = None) -> str | None:
    """Hash ``password`` with the configured bcrypt rounds."""

    if password is None:
        return None
    settings = get_settings()
    password_rounds = settings.pass_rounds
    salt = bcrypt.gensalt(rounds=password_rounds)
    password_hash = bcrypt.hashpw(password.encode(DEFAULT_FILE_ENCODING), salt)
    return password_hash.decode(DEFAULT_FILE_ENCODING)


def password_verify(password: str | None, hashed: str | None) -> bool:
    """Return ``True`` when ``password`` matches the stored hash."""

    if not password or not hashed:
        return False
    return bcrypt.checkpw(password.encode(DEFAULT_FILE_ENCODING), hashed.encode(DEFAULT_FILE_ENCODING))
