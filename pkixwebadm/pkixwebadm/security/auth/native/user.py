from __future__ import annotations

from types import SimpleNamespace

from ..identity import Identity
from ..user import UserRecord, UserRepository
from pkixwebadm import get_settings


BOOTSTRAP_ADMIN_ID = "bootstrap-admin"
BOOTSTRAP_ADMIN_DISPLAY_NAME = "Administrator"


def build_bootstrap_admin_record() -> UserRecord:
    settings = get_settings()
    return SimpleNamespace(
        identity=Identity(
            user_id=BOOTSTRAP_ADMIN_ID,
            username=settings.bootstrap_admin_user,
            display_name=BOOTSTRAP_ADMIN_DISPLAY_NAME,
        ),
        verification_token=settings.bootstrap_admin_pass_hash,
    )


class NativeUserRepository(UserRepository):
    "Native implementation of a user repository"

    def get_by_username(self, username: str) -> UserRecord | None:
        if not username:
            return None
        settings = get_settings()
        if settings.bootstrap_admin_user == username:
            return build_bootstrap_admin_record()
        return None
