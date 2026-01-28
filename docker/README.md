# Docker Stack

This directory contains the static stack index used by the Docker demo.

The WiFiEqu entrypoints now live under:
- `wifiequ/linux/docker/entrypoint-backend.sh`
- `wifiequ/web-angular/docker/entrypoint-frontend.sh`

## One-command tour with Docker Compose

From the repository root:

```bash
docker compose up --build
```

That builds the stack images and binds the services to the host:

| Service | Host port | Container port | Notes |
| --- | --- | --- | --- |
| `stack-index` | `8080` | `80` | Static landing page linking to all running services. |
| `heapmonj-backend` | `8081` | `8080` | Spring Boot heap monitor backend. |
| `heapmonj-frontend` | `8082` | `80` | Angular heap monitor UI (nginx). |
| `null-cipher` | `8083` | `80` | Static HTML/JS cipher demo. |
| `pkixwebadm` | `8084` | `8080` | FastAPI certificate inventory (dev preview). |
| `pwa-llm-poc-backend` | `8085` | `5000` | Flask + Ollama bridge (requires host Ollama). |
| `pwa-llm-poc-frontend` | `8086` | `80` | PWA UI (nginx). |
| `slideshow-server` | `8087` | `8080` | Serves the bundled slideshow demo. |
| `triangle-peg-game-react` | `8088` | `80` | React SPA for the peg game (static nginx serve). |
| `wifiequ-backend` | `8089` | `8080` | Runs the mock-mode WiFiEqu JSON API (default token baked into compose). |
| `wifiequ-frontend` | `8090` | `80` | Serves the Angular WiFiEqu UI via nginx with stats-key injection (shares the default token). |

Useful follow-ups:

```bash
docker compose up -d                 # start in the background
docker compose logs -f slideshow-server
docker compose logs -f wifiequ-backend
docker compose stop wifiequ-backend  # stop a single service
docker compose down                  # stop everything and remove the containers
```

Note: the `pwa-llm-poc-backend` service expects Ollama to be running on the host. Use `OLLAMA_HOST=0.0.0.0:11434 ollama serve` so containers can reach it.

## Installing Docker via snap (Ubuntu)

```bash
sudo snap install docker
sudo systemctl enable --now snap.docker.dockerd
sudo usermod -aG docker "$USER"
newgrp docker                        # refresh the current shell session
```

Without the `newgrp` (or a full logout/login) you will see `permission denied` errors when Compose talks to `/var/run/docker.sock`. After joining the `docker` group the standard `docker compose ...` commands work without `sudo`.

## Manual builds

You can still build the images individually if you prefer:

```bash
docker build -t slideshow-server -f slideshow-server/Dockerfile .
docker build -t wifiequ-backend -f wifiequ/Dockerfile .
docker build -t wifiequ-frontend -f wifiequ/web-angular/Dockerfile .
docker build -t heapmonj-backend -f heapmonj/backend/Dockerfile .
docker build -t heapmonj-frontend -f heapmonj/frontend/Dockerfile .
docker build -t pwa-llm-poc-backend -f pwa-llm-poc/backend/Dockerfile pwa-llm-poc
docker build -t pwa-llm-poc-frontend -f pwa-llm-poc/frontend/llm-spa/Dockerfile pwa-llm-poc/frontend/llm-spa
```

Those commands live in the project READMEs as well; Compose simply orchestrates both builds and runs with the correct volume mounts and environment.

---

- Back to the [repository overview](../README.md)
