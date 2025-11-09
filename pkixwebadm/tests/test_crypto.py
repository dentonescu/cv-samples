import logging

from pkixwebadm import password_hash, password_verify

logger = logging.getLogger(__name__)

def test_password_hash():
    pw_plaintext = "this-is-not-a-real-password"
    pw_hashed = password_hash(pw_plaintext)
    logger.debug("pw_plaintext: %s, pw_hashed: %s", pw_plaintext, pw_hashed)
    assert pw_hashed is not None
    assert pw_plaintext != pw_hashed

def test_password_verify():
    pw = "likkle but wi tallawah"
    pw_hashed = password_hash(pw)
    assert password_verify(pw, pw_hashed)