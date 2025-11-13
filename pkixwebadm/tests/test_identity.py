import pytest
from pkixwebadm import Identity
from pydantic import ValidationError


def test_identity_is_immutable():
    identity = Identity(user_id="1", username="admin")
    # Pydantic v1 mutability checks raise TypeError; v2 raises ValidationError.
    with pytest.raises((TypeError, ValidationError)):
        identity.username = "new"


def test_identity_has_role_helper():
    identity = Identity(user_id="1", username="admin", roles=("admin", "operator"))
    assert identity.has_role("admin")
    assert not identity.has_role("guest")
