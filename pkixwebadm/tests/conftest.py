import logging
import sys
from pathlib import Path

PROJECT_SRC = Path(__file__).resolve().parents[1] / "pkixwebadm"
if PROJECT_SRC.exists():
    sys.path.insert(0, str(PROJECT_SRC))

def pytest_configure(config):
    logging.basicConfig(level=logging.DEBUG, format="%(levelname)s %(name)s: %(message)s")
