import pytest
from pydantic import ValidationError

from pkixwebadm import Credentials

def test_credentials_is_immutable():
    credentials = Credentials(username="admin", 
                              password="not-the-real-password")
    # Pydantic v1 raises TypeError, v2 raises ValidationError for frozen models.
    with pytest.raises((TypeError, ValidationError)):
        credentials.password = "changed"


def test_credentials_supports_multiple_fields():
    credentials = Credentials(username="admin", 
                              password="not-the-real-password", 
                              token="abc", 
                              key_file="/tmp/key")
    assert credentials.username == "admin"
    assert credentials.password == "not-the-real-password"
    assert credentials.token == "abc"
    assert credentials.key_file == "/tmp/key"
