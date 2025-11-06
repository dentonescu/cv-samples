import logging

from libdmotservices import get_resource_base_name

logger = logging.getLogger(__name__)

def test_get_resource_base_name():
    url_1: str = '/just/another/unit/test'
    url_2: str = '\\under\\my\\desk'
    base_name_1: str = get_resource_base_name(url_1)
    logger.debug('url_1=%s, base_name_1=%s', url_1, base_name_1)
    base_name_2: str = get_resource_base_name(url_2)
    logger.debug('url_2=%s, base_name_2=%s', url_2, base_name_2)
    assert base_name_1 == 'test'
    assert base_name_2 == 'desk'