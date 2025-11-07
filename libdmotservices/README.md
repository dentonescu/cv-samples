# libdmotservices

**libdmotservices** is my personal multi-language code library — a curated collection of small, reusable utilities written in **C**, **Java**, and now **Python** (with more languages to come). It showcases how I approach **cross-language API design**, **version management**, and **clean software architecture** across ecosystems.

## Project Summary
This project demonstrates my ability to:
- Design modular, testable libraries in multiple languages.
- Centralize versioning across heterogeneous builds using a top-level `Makefile` and `VERSION` file.
- Automate builds, tests, and examples for consistency and reproducibility.
- Write maintainable documentation aimed at clarity and professional presentation.

## Structure
| Path | Description |
|------|--------------|
| [`c/`](c/README.md) | Portable C modules (signals, logging, terminal equalizer). |
| [`java/`](java/README.md) | Java modules (`dmot-core` and `dmot-servlet-javax`) with Ant/Maven builds. |
| [`python/`](python/README.md) | Python helpers that complement the CLI tooling used across the portfolio. |
| [`js/`](js/README.md) | Browser-friendly utilities (FormData helpers, logging) bundled for the front-end samples. |
| `VERSION` | Central version file used across all subprojects. |
| `Makefile` | Top-level orchestrator that builds, tests, and runs examples across languages. |

## Highlights
- Portable codebases: compiles cleanly across Linux systems.
- Full test coverage with CMocka (C), JUnit (Java), and pytest (Python).
- Minimal dependencies and clear build instructions.
- API-first mindset: code is structured for reusability and clarity.
- Automated via Make/Ant/Maven with fail-fast targets; GitHub Actions (`../.github/workflows/cv-samples-ci.yml`) exercises all supported languages with the same revision metadata that `dev.sh` passes locally.
- Featured sample: an ANSI equalizer capable of labelled channels, also used in my WiFiEqu Linux monitor project.
- Shared Python utilities (`libdmotservices.args.print_help_all`, `libdmotservices.string.get_resource_base_name`) power CLI ergonomics and FastAPI integrations in pkixwebadm.
- New JavaScript helpers (logger + FormData utilities) ship as a single vendored bundle for pkixwebadm’s drag-and-drop UI.

## Build Overview
```bash
# Build everything (C + Java)
make clean all

# Run tests and examples
make test
make example-demo
```

Each subproject ships with its own README and can be built independently.

## Why this exists

This repo collects tiny, battle-tested snippets I reach for repeatedly. They’re intentionally small, readable, and dependency-light to work well as CV samples.

## What this demonstrates
- Cross-language integration and build orchestration.
- A disciplined, maintainable approach to multi-language software design.
- Documentation and automation skills relevant to API development roles.
- Ability to share utility code across portfolio projects (WiFiEqu, pkixwebadm, slideshow-server).

## Status

Active work-in-progress. APIs may evolve as other portfolio samples (like [WiFiEqu](../wifiequ/README.md) and [pkixwebadm](../pkixwebadm/README.md)) grow.

## Screenshot

![Build screenshot](libdmotservices.png)

## Related
- Developer notes: [NOTES.md](NOTES.md)
- Back to [portfolio overview](../README.md)

---
**License:** [MIT](LICENSE)
