# Backend

Flask API that proxies prompts to Ollama and returns human‑readable output.

## Run locally
```bash
python3 app.py
```

Default host/port:
- `0.0.0.0:5000`

## Environment variables
- `PWA_LLM_BIND_ADDRESS` (default `0.0.0.0`)
- `PWA_LLM_PORT` (default `5000`)
- `PWA_LLM_MAX_PROMPT_LEN` (default `4000`)
- `OLLAMA_URL` (default `http://localhost:11434`)
- `OLLAMA_MODEL` (default `gemma3`)
- `OLLAMA_TIMEOUT_SECONDS` (default `300`)

## Ollama requirement
Ollama must be running locally and the model should be pulled:
```bash
ollama pull gemma3
OLLAMA_HOST=0.0.0.0:11434 ollama serve
```

The bind to `0.0.0.0` is required when the backend runs inside Docker and needs to reach the host.

## Navigation
- Back to the project overview: [../README.md](../README.md)
