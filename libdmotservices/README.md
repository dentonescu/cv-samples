# libdmotservices

**libdmotservices** is a personal, multi-language library of small, reusable utilities I’ve built to make everyday programming tasks easier.  
Each module includes unit tests and minimal examples.

Although the project began with Java utilities, my long-term goal is to populate it with reusable code from **every language I work with**.
Each language resides in its own subdirectory.

**License:** [MIT](LICENSE)

## Languages

- [C](c) — portable utilities with a Linux build (signals, logging, ANSI helpers)
- [Java](java) — core helpers and a legacy `javax.servlet` module

## What’s here (high level)

- **Signals:** small math helpers (e.g., bounded sine “dBm” generator) used by demos like WiFiEqu (coming soon).
- **Logging:** tiny `printf`-style logger with levels.
- **Examples / Tests:** each language has self-contained examples and unit tests.

## Why this exists

This repo collects tiny, battle-tested snippets I reach for repeatedly.  
They’re intentionally small, readable, and dependency-light to work well as CV samples.

## Status

Active work-in-progress. APIs may evolve. Each subproject has its own README with build instructions.

