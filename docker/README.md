# Docker Stack

This directory contains helper scripts for the containerised demos:

- [`wifiequ-entrypoint.sh`](wifiequ-entrypoint.sh) — performs last-minute config tweaks for the WiFiEqu daemon (injects an API token when needed, then execs the daemon).
- [`nginx.conf`](../heapmonj/frontend/nginx.conf) — used by the heapmonj frontend image.
> Note: WiFiEqu now ships dedicated entrypoints under `wifiequ/linux/docker/entrypoint-backend.sh` and `wifiequ/web-angular/docker/entrypoint-frontend.sh`. This folder retains legacy scripts for reference.

## One-command tour with Docker Compose

From the repository root:

```bash
docker compose up --build
```

That builds the stack images and binds the services to the host:

| Service | Host port | Container port | Notes |
| --- | --- | --- | --- |
| `stack-index` | `8080` | `80` | Static landing page linking to all running services. |
| `slideshow-server` | `8083` | `8080` | Serves the bundled slideshow demo. |
| `wifiequ-backend` | `8084` | `8080` | Runs the mock-mode WiFiEqu JSON API (default token baked into compose). |
| `wifiequ-frontend` | `8085` | `80` | Serves the Angular WiFiEqu UI via nginx with stats-key injection (shares the default token). |
| `heapmonj-backend` | `8081` | `8080` | Spring Boot heap monitor backend. |
| `heapmonj-frontend` | `8082` | `80` | Angular heap monitor UI (nginx). |

Useful follow-ups:

```bash
docker compose up -d                 # start in the background
docker compose logs -f slideshow-server
docker compose logs -f wifiequ-backend
docker compose stop wifiequ-backend  # stop a single service
docker compose down                  # stop everything and remove the containers
```

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
```

Those commands live in the project READMEs as well; Compose simply orchestrates both builds and runs with the correct volume mounts and environment.

---

- Back to the [repository overview](../README.md)
