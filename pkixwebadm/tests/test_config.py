import logging
from pkixwebadm import get_settings, Settings

logger = logging.getLogger(__name__)

def test_get_settings():
    settings: Settings = get_settings()
    logger.debug("settings=%s", settings)
    if settings:
        logger.debug("settings.model_config=%s", dict(settings.model_config))
    assert settings is not None
    assert settings.model_config is not None

def test_secret_key_from_env(monkeypatch):
    """The loader should pick up PKIXWA_SECRET_KEY when set in the environment."""
    monkeypatch.setenv("PKIXWA_SECRET_KEY", "test-secret")
    settings = get_settings()
    assert settings.secret_key == "test-secret"
