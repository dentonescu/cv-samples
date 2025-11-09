import logging
import sys
from pathlib import Path
from pkixwebadm import PROJECT_NAME, configure_logging

PROJECT_SRC = Path(__file__).resolve().parents[1] / PROJECT_NAME
if PROJECT_SRC.exists():
    sys.path.insert(0, str(PROJECT_SRC))

def pytest_configure(config):
    configure_logging(logging.DEBUG)
