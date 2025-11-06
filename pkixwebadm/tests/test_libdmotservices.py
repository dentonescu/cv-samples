import logging

from libdmotservices import get_resource_base_name

logger = logging.getLogger(__name__)

def test_libdmotservices_utility_classes():
    url = "https://www.example.com/some/thing/which/does/not/exist"
    base_name = get_resource_base_name(url)
    logger.debug("url=%s, base_name=%s", url, base_name)
    assert base_name == "exist"