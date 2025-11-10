#!/usr/bin/env python3
"""Generate a disposable password + bcrypt hash for bootstrap workflows."""

from __future__ import annotations

import bcrypt
import os
import secrets

from pkixwebadm import password_hash


def _read_int(name: str, default: int) -> int:
    value = os.getenv(name)
    if value is None:
        return default
    try:
        return int(value)
    except ValueError:  # pragma: no cover - defensive guard
        return default


def main() -> None:
    pw_len = _read_int("PKIXWA_PASS_LEN", 16)
    pw = secrets.token_urlsafe(pw_len)
    pw_hashed = password_hash(pw)
    print(f"PKIXWA_BOOTSTRAP_ADMIN_PASS={pw}")  # Do not record in .env
    print(f"PKIXWA_BOOTSTRAP_ADMIN_PASS_HASH={pw_hashed}")


if __name__ == "__main__":
    main()
