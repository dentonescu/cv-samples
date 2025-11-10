"""Settings management for pkixwebadm."""

from libdmotservices import DEFAULT_FILE_ENCODING

try:
    from pydantic_settings import BaseSettings, SettingsConfigDict
except ImportError:  # pragma: no cover - fallback for environments without pydantic-settings
    from pydantic import BaseSettings # v1: BaseSettings was in the pydantic package
    SettingsConfigDict = None

from typing import Literal

ENV_PREFIX = "PKIXWA_"
ENV_FILE = ".env"
ENV_FILE_ENCODING = DEFAULT_FILE_ENCODING
MODEL_CONFIG = dict(
    env_prefix=ENV_PREFIX,
    env_file=ENV_FILE,
    env_file_encoding=ENV_FILE_ENCODING,
)

class Settings(BaseSettings):
    """Project configuration sourced from environment variables and ``.env``."""

    auth_method: Literal["native", "oidc"] = "native"  # PKIXWA_AUTH_METHOD
    bootstrap_admin_user: str | None = None # PKIXWA_BOOTSTRAP_ADMIN_USER
    bootstrap_admin_pass_hash: str | None = None # PKIXWA_BOOTSTRAP_ADMIN_PASS_HASH
    dev_mode: bool = True  # PKIXWA_DEV_MODE
    database_url: str = "sqlite:///var/data/app.db"  # PKIXWA_DATABASE_URL
    host: str = "localhost"  # PKIXWA_HOST
    pass_rounds: int = 12  # PKIXWA_PASS_ROUNDS
    port: int = 8080  # PKIXWA_PORT
    secret_key: str | None = None  # PKIXWA_SECRET_KEY
    session_cookie_name: str = "PKIXWA_SESSION"  # PKIXWA_SESSION_COOKIE_NAME
    session_ttl_seconds: int = 86400  # PKIXWA_SESSION_TTL_SECONDS

    model_config = SettingsConfigDict(**MODEL_CONFIG) if SettingsConfigDict else MODEL_CONFIG

    if not SettingsConfigDict:
        # Pydantic v1 still relies on the nested Config class.
        class Config:
            env_prefix = ENV_PREFIX
            env_file = ENV_FILE
            env_file_encoding = ENV_FILE_ENCODING

def get_settings() -> Settings:
    """Return a freshly loaded :class:`Settings` instance."""

    return Settings()
