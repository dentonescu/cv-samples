"""Ollama integration helpers for pwa-llm-poc."""

from __future__ import annotations

import requests

from typing import Any, Dict

#
# local imports
#

try:
    from .config import (
        OLLAMA_URL,
        OLLAMA_MODEL,
        OLLAMA_TIMEOUT_SECONDS,
        SYSTEM_PROMPT,
    )
except ImportError:  # pragma: no cover - fallback for direct script execution
    from config import (
        OLLAMA_URL,
        OLLAMA_MODEL,
        OLLAMA_TIMEOUT_SECONDS,
        SYSTEM_PROMPT,
    )

#
# internals
#

def _build_prompt(user_prompt: str) -> str:
    return f"{SYSTEM_PROMPT}\n\nUser prompt:\n{user_prompt}\n"


def _call_ollama(prompt: str) -> Dict[str, Any]:
    response = requests.post(
        f"{OLLAMA_URL}/api/generate",
        json={
            "model": OLLAMA_MODEL,
            "prompt": prompt,
            "stream": False,
        },
        timeout=OLLAMA_TIMEOUT_SECONDS,
    )
    response.raise_for_status()
    return response.json()

#
# externals
#

def generate_llm_output(user_prompt: str) -> str:
    """Return a human-readable response for the provided prompt."""
    prompt = _build_prompt(user_prompt)
    result = _call_ollama(prompt)
    raw_response = result.get("response", "")
    if not raw_response:
        raise ValueError("LLM returned an empty response.")
    return str(raw_response)

#
# exports
#

__all__ = [
    "generate_llm_output",
]
