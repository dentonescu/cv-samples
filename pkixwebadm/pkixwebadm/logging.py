import logging
from copy import deepcopy
from logging.config import dictConfig

from . import PROJECT_NAME

DEFAULT_LOG_LEVEL_NAME = "INFO"
CONSOLE_ERROR = "stderr"
CONSOLE_NORMAL = "stdout"

class _MaxLevelFilter(logging.Filter):
    """Filter that lets records through up to and including ``max_level``."""

    def __init__(self, name: str = "", max_level: int = logging.INFO) -> None:
        super().__init__(name)
        self.max_level = max_level

    def filter(self, record: logging.LogRecord) -> bool:
        return record.levelno <= self.max_level

LOGGING_CONFIG = {
    "version": 1,
    "disable_existing_loggers": False,
    "filters": {
        "max_info": {
            "()": "pkixwebadm.logging._MaxLevelFilter",
            "max_level": logging.ERROR - 1,
        },
    },
    "formatters": {
        "console": {
            "format": "%(asctime)s | %(levelname)s | %(name)s | %(message)s",
        },
    },
    "handlers": {
        CONSOLE_NORMAL: {
            "class": "logging.StreamHandler",
            "stream": "ext://sys.stdout",
            "formatter": "console",
            "filters": ["max_info"],
        },
        CONSOLE_ERROR: {
            "class": "logging.StreamHandler",
            "formatter": "console",
            "level": "ERROR",
        },
    },
    "loggers": {
        PROJECT_NAME: {"handlers": [CONSOLE_NORMAL, CONSOLE_ERROR], "level": DEFAULT_LOG_LEVEL_NAME},
        "uvicorn.error": {
            "handlers": [CONSOLE_NORMAL, CONSOLE_ERROR],
            "level": DEFAULT_LOG_LEVEL_NAME,
            "propagate": False,
        },
        "uvicorn.access": {
            "handlers": [CONSOLE_NORMAL, CONSOLE_ERROR],
            "level": DEFAULT_LOG_LEVEL_NAME,
            "propagate": False,
        },
    },
    "root": {"handlers": [CONSOLE_NORMAL, CONSOLE_ERROR], "level": "WARNING"},
}

def configure_logging(level: int | None = None) -> None:
    config = deepcopy(LOGGING_CONFIG)
    if level:
        config["loggers"][PROJECT_NAME]["level"] = logging.getLevelName(level)
    dictConfig(config)
