"""Runtime configuration for the pwa-llm-poc backend."""

import os

ENV_PWA_LLM_BIND_ADDRESS = os.getenv("PWA_LLM_BIND_ADDRESS", "0.0.0.0")
ENV_PWA_LLM_MAX_PROMPT_LEN = int(os.getenv("PWA_LLM_MAX_PROMPT_LEN", "4000"))
ENV_PWA_LLM_PORT = int(os.getenv("PWA_LLM_PORT", "5000"))

OLLAMA_URL = os.getenv("OLLAMA_URL", "http://localhost:11434")
OLLAMA_MODEL = os.getenv("OLLAMA_MODEL", "gemma3")
OLLAMA_TIMEOUT_SECONDS = float(os.getenv("OLLAMA_TIMEOUT_SECONDS", "300"))

SYSTEM_PROMPT = (
    "You are a helpful assistant. "
    "Return human-readable text. "
    "No preamble, no labels. "
    "Do not include code fences or extra text. "
    "Refuse unsafe or illegal requests."
)


#
# exports
#
__all__ = [
    "ENV_PWA_LLM_BIND_ADDRESS",
    "ENV_PWA_LLM_MAX_PROMPT_LEN",
    "ENV_PWA_LLM_PORT",
    "OLLAMA_URL",
    "OLLAMA_MODEL",
    "OLLAMA_TIMEOUT_SECONDS",
    "SYSTEM_PROMPT",
]
