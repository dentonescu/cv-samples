# conftest.py

import logging

def pytest_configure(config):
    logging.basicConfig(level=logging.DEBUG, format="%(levelname)s %(name)s: %(message)s")
