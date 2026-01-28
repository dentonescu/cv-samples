"""Flask API for the pwa-llm-poc backend."""

from __future__ import annotations

import logging

from http import HTTPStatus
from flask import Flask, jsonify, request
from typing import Any, Dict

#
# local imports
#

try:
    from .config import (
        ENV_PWA_LLM_BIND_ADDRESS,
        ENV_PWA_LLM_MAX_PROMPT_LEN,
        ENV_PWA_LLM_PORT,
    )
    from .llm import generate_llm_output
except ImportError:  # pragma: no cover - fallback for direct script execution
    from config import (
        ENV_PWA_LLM_BIND_ADDRESS,
        ENV_PWA_LLM_MAX_PROMPT_LEN,
        ENV_PWA_LLM_PORT,
    )
    from llm import generate_llm_output

#
# internals
#

def _error_response(message: str, status_code: int):
    return jsonify({"error": message}), status_code


def _get_prompt(payload: Dict[str, Any]) -> str | None:
    prompt = payload.get("prompt")
    if not isinstance(prompt, str):
        return None
    return prompt.strip()

#
# app
#

logger = logging.getLogger(__name__)

def create_app() -> Flask:
    """Create and configure the Flask application."""
    app = Flask(__name__)

    @app.post("/api/v1/generate")
    def generate():
        if not request.is_json:
            return _error_response("Expected JSON body.", HTTPStatus.BAD_REQUEST)

        payload = request.get_json(silent=True) or {}
        prompt = _get_prompt(payload)
        if not prompt:
            return _error_response("Prompt must be a non-empty string.", HTTPStatus.BAD_REQUEST)
        if len(prompt) > ENV_PWA_LLM_MAX_PROMPT_LEN:
            return _error_response(
                f"Prompt exceeds {ENV_PWA_LLM_MAX_PROMPT_LEN} characters.", HTTPStatus.BAD_REQUEST
            )

        try:
            logger.debug(f"prompt: {prompt}")
            output = generate_llm_output(prompt)
        except ValueError as exc:
            return _error_response(str(exc), HTTPStatus.BAD_REQUEST)
        except Exception:  # noqa: BLE001 - surface a controlled error
            return _error_response("Failed to generate a response.", HTTPStatus.INTERNAL_SERVER_ERROR)

        return jsonify({"output": output})

    return app


app = create_app()

#
# main
#

if __name__ == "__main__":
    app.run(host=ENV_PWA_LLM_BIND_ADDRESS, port=ENV_PWA_LLM_PORT, debug=True)
