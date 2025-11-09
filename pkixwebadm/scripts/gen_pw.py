#!/usr/bin/env python3
"""Generate a disposable password + bcrypt hash for bootstrap workflows."""

from __future__ import annotations

import os
import secrets

from passlib.hash import bcrypt


def _read_int(name: str, default: int) -> int:
    value = os.getenv(name)
    if value is None:
        return default
    try:
        return int(value)
    except ValueError:  # pragma: no cover - defensive guard
        return default


def main() -> None:
    password_len = _read_int("PKIXWA_PASS_LEN", 16)
    password_rounds = _read_int("PKIXWA_PASS_ROUNDS", 12)
    password = secrets.token_urlsafe(password_len)
    password_hash = bcrypt.using(rounds=password_rounds).hash(password)
    print(f"PKIXWA_BOOTSTRAP_ADMIN_PASS={password}")  # Do not record in .env
    print(f"PKIXWA_BOOTSTRAP_ADMIN_PASS_HASH={password_hash}")


if __name__ == "__main__":
    main()
