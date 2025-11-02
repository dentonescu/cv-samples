# cv-samples

A compact portfolio of small, self‑contained code projects that complement my CV. Each folder contains either a standalone app or a minimal library with examples and tests. The focus is clarity, correctness, and cross‑language thinking. Projects are developed in their own staging repositories before they are published here, so every sample arrives with its build scripts, docs, and local checks already exercised.

> Live previews and links: [**cv‑samples.vercel.app**](https://cv-samples.vercel.app)

[![cv-samples CI](https://github.com/dentonescu/cv-samples/actions/workflows/cv-samples-ci.yml/badge.svg?branch=main)](.)
![License](https://img.shields.io/github/license/dentonescu/cv-samples)

## About me
I’m a software engineer who enjoys building neat, well‑scoped utilities and visual demos across languages (C, Java, Python, TypeScript/JavaScript, and Angular). I care about readable code, reproducible builds, and pragmatic architecture.

## Projects

| Project | What it is | Primary skills |
|---|---|---|
| **[libdmotservices](libdmotservices/README.md)** | Personal multi‑language utility library with unit tests and examples. | C, Java, Makefile, Unit Testing |
| **[null-cipher](null-cipher/README.md)** | Single‑page visual demo of a simple cipher (encrypt/decrypt) in the browser. | JavaScript, HTML, CSS |
| **[slideshow-server](slideshow-server/README.md)** | Tiny Python web server that generates responsive media slideshows. | Python, HTML, JS |
| **[WiFiEqu](wifiequ/README.md)** (in progress) | Terminal Wi‑Fi channel “equalizer” visualizer with a C JSON daemon, Windows service, and Angular web UI. The Linux daemon streams live/mock readings over SSE, honours configurable refresh cadences, and secures `/api/v1/stats` with an API key. | C, ANSI/terminal, JSON APIs, C#, Angular |
| **[Terraglean](terraglean/README.md)** (planned) | Interactive map showing world data layers (time, weather, country stats). | Java, Spring Boot, Leaflet.js |

## Tooling & automation
- `dev.sh` in the repository root orchestrates dependency setup, builds, tests, and demo runs from a single entrypoint. It is the quickest way for a reviewer to get everything compiling and deployed locally (`./dev.sh --install-deps --install-prj --build --run-tests --run-examples` mirrors the CI run). Pass `--iface <name>` if your Wi-Fi device is not `wlan0` (e.g. `--iface wlp2s0`), and run the examples with `sudo` when the wireless scan demo needs hardware access.
- `.github/workflows/cv-samples-ci.yml` mirrors the `dev.sh` flow on GitHub Actions: dependencies → build → tests → example runs. The pipeline ensures every project stays buildable on a clean Ubuntu runner (the Wi-Fi scan demo skips gracefully when no wireless interface is available).
- `.gitattributes` files have been introduced across the projects to normalise line endings and enforce consistent attributes, making diffs reliable across platforms.

## How to use this repository
- Every project folder has its own `README.md` with quick build or run instructions.
- Prefer `./dev.sh --install-deps --install-prj --build --run-tests --run-examples` for the full local check, or run the corresponding CI workflow manually from the Actions tab if you want to watch the hosted logs.
- Code is intentionally small and instructional—suited for reading and discussion.
- Nothing here is used in production customer systems.

## License
All content is provided under the license in `/LICENSE`. You’re free to read and learn from the code. If you use parts of it, please include attribution.


---

Check back regularly — new samples will be added and existing ones improved as the gallery evolves.
