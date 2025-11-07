import logging
from logging.config import dictConfig
from . import PROJECT_NAME

DEFAULT_LOG_LEVEL_NAME = "INFO"

LOGGING_CONFIG = {
    "version": 1,
    "disable_existing_loggers": False,
    "formatters": {
        "console": {
            "format": "%(asctime)s | %(levelname)s | %(name)s | %(message)s",
        },
    },
    "handlers": {
        "stderr": {
            "class": "logging.StreamHandler",
            "formatter": "console",
        },
    },
    "loggers": {
        PROJECT_NAME: {"handlers": ["stderr"], "level": DEFAULT_LOG_LEVEL_NAME},
        "uvicorn.error": {"handlers": ["stderr"], "level": DEFAULT_LOG_LEVEL_NAME, "propagate": False},
        "uvicorn.access": {"handlers": ["stderr"], "level": DEFAULT_LOG_LEVEL_NAME, "propagate": False},
    },
    "root": {"handlers": ["stderr"], "level": "WARNING"},
}

def configure_logging(level: int | None = None) -> None:
    config = LOGGING_CONFIG.copy()
    if level:
        config["loggers"][PROJECT_NAME]["level"] = logging.getLevelName(level)
    dictConfig(config)