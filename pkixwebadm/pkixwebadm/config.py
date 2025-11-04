# config.py

from pkixwebadm import DEFAULT_FILE_ENCODING

from pydantic_settings import BaseSettings, SettingsConfigDict


class Settings(BaseSettings):
    """Project configuration sourced from environment variables and ``.env``.

    Each attribute maps to a ``PKIXWA_*`` variable; when unset, the defaults
    provide a functional local development configuration.
    """

    host: str = "localhost"                    # PKIXWA_HOST
    port: int = 8080                           # PKIXWA_PORT
    dev_mode: bool = True                      # PKIXWA_DEV_MODE
    database_url: str = "sqlite:///var/data/app.db"  # PKIXWA_DATABASE_URL
    secret_key: str | None = None              # PKIXWA_SECRET_KEY
    session_cookie_name: str = "PKIXWA_SESSION"      # PKIXWA_SESSION_COOKIE_NAME
    session_ttl_seconds: int = 86400           # PKIXWA_SESSION_TTL_SECONDS

    model_config = SettingsConfigDict(
        env_prefix="PKIXWA_",
        env_file=".env",
        env_file_encoding=DEFAULT_FILE_ENCODING
    )

def get_settings() -> Settings:
    return Settings()
