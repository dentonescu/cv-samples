# NOTES

Developer notes for **libdmotservices** — the umbrella project that houses the reusable C and Java utilities backing several samples (WiFiEqu, CLI demos, servlet helpers).

## Current File Layout
```
libdmotservices/
├─ README.md                     # Project overview and integration notes
├─ NOTES.md                      # Cross-language developer log
├─ VERSION                       # Library release tag
├─ Makefile                      # Orchestrates C and Java builds/tests
├─ gen-version.py                # Shared VERSION bumping helper
├─ .gitattributes / .gitignore
├─ dist/                         # Published artifacts (static libs, jars, demos)
├─ c/
│  ├─ README.md / NOTES.md
│  ├─ Makefile
│  ├─ .vscode/
│  └─ linux/
│     ├─ README.md / NOTES.md
│     ├─ Makefile
│     ├─ include/dmot/*.h        # Public headers (math, string, time, ...)
│     ├─ src/
│     │  ├─ datatypes/datatypes.c
│     │  ├─ math/{math.c, signals.c, mathx.d}
│     │  ├─ string/{string.c, stringx.d}
│     │  ├─ time/{time.c, timex.d}
│     │  └─ util/log.c
│     ├─ tests/                  # CMocka suites (test_math.c, ...)
│     ├─ examples/               # Sample programs (ex_signals.c, ...)
│     └─ img/equalizer.png
├─ java/
│  ├─ README.md / NOTES.md
│  ├─ build.xml / build.sh / build.cmd
│  ├─ log4j2.xml
│  ├─ mock/                      # Test doubles
│  ├─ dmot-core/
│  │  ├─ README.md / NOTES.md
│  │  ├─ build.xml / pom.xml
│  │  ├─ dep/jars/json-20231013.jar
│  │  └─ src/
│  │     ├─ main/java/dev/dmotservices/**  # Core API & utilities
│  │     └─ test/java/dev/dmotservices/**  # JUnit tests
│  └─ dmot-servlet-javax/
│     ├─ README.md / NOTES.md
│     ├─ build.xml
│     └─ src/
│        ├─ main/java/dev/dmotservices/servlet/**  # Servlet adapters
│        └─ test/java/dev/dmotservices/servlet/**  # Tests
├─ libdmotservices.logo
└─ libdmotservices.png
```

## Project Snapshot
- Shared assets: top-level `Makefile` syncs version numbers from `VERSION`, kicks off language-specific builds, runs tests, and copies artifacts into `dist/`.  
- Cross-project usage: the equalizer renderer feeds WiFiEqu demos; Java utilities support planned Spring/Terraglean services.  
- Documentation: language-specific READMEs (`c/`, `java/`) focus on build/run, while this file tracks overarching plans and design choices.

## Development Plan
1. **Versioning & release hygiene**  
   - Keep `gen-version.py` as the single source of truth for Git metadata; call it from each subproject’s `Makefile`.  
   - Automate changelog fragments per language, then roll them up into a root `CHANGELOG.md` when publishing.  
   - Add release packaging target that produces language-specific tarballs plus a unified manifest.
2. **C + Java parity**  
   - Ensure both languages expose comparable modules (logging, timing, equalizer abstractions).  
   - Document the shared conceptual model (signal channels, logging levels) so other languages can join later.  
   - Evaluate generating bindings (e.g., C headers → Java JNI) only after both sides stabilise.
3. **Testing discipline**  
   - Run `make test` (root) to execute C CMocka suites and Java JUnit suites sequentially; surface combined coverage metrics.  
   - Provide smoke demos (`make example-demo`) that tie into consumers (WiFiEqu) for regression checks.  
   - Add pre-commit hooks or CI steps to guard header/ABI compatibility.
4. **Documentation & samples**  
   - Expand language-specific NOTES to capture design context (see `c/NOTES.md`, `java/NOTES.md`).  
   - Highlight cross-project integration examples (e.g., WiFiEqu CLI screenshot) and link them from this file.  
   - Consider publishing API references (Doxygen / Javadoc) under `docs/`.
5. **Future languages**  
   - Keep structure ready for additional folders (`python/`, `typescript/`).  
   - Document expectations (tests, examples, packaging) so new languages plug in consistently.

## Domain Notes
- Root `Makefile` orchestrates per-language builds via `$(MAKE) -C <dir>`—avoid adding language-specific tooling here; keep those within subdirectories.  
- Version propagation relies on text replacements; ensure newline conventions remain consistent (LF).  
- When updating shared assets (logos, docs), keep file paths stable to avoid breaking downstream references (e.g., WiFiEqu docs embed the equalizer image).

## Implementation Timeline
- **2021-10 – 2023-04: Early Java library era**  
  The original Maven project took shape, groupIds were refined, and Jakarta alignment experiments landed before the codebase paused for a while.
- **2025-10-11 – 2025-10-15: Multi-language reboot**  
  The project relaunched with versioned Java modules, Ant orchestration, and a split between `dmot-core` and `dmot-servlet-javax`. Documentation and screenshots reintroduced the library, while the C side surfaced with Makefiles, unit tests, and sine-wave generators.
- **2025-10-14 – 2025-10-18: Equalizer & build orchestration**  
  UI work introduced the equalizer rendering pipeline, threading demos, and console sizing, while master Makefile and version header tooling unified builds across languages.
- **2025-10-19 – 2025-10-26: Feature expansion & releases**  
  Timestamp helpers, string utilities, and labelled equalizer channels matured the C API, leading to a trio of releases that delivered channel hiding, smoothing, and stable redraws in time for WiFiEqu integration.
- **2025-10-29 – 2025-11-02: Versioning parity & polish**  
  Cross-language semantic version helpers, documentation refreshes, and build corrections cemented the current state, keeping the shared library ready for downstream consumers.

## Testing Checklist
- `make clean all` and `make test` succeed at the root on a clean machine.  
- Language-specific tests/demos continue to run in isolation (`make -C c test`, `make -C java test`).  
- Dist artifacts (`dist/*.tar.gz`, `.jar`) carry the correct semantic versions and manifest metadata.

## Related Documentation
- Project overview: [README.md](README.md)  
- C modules: [c/README.md](c/README.md)  
- C developer notes: [c/NOTES.md](c/NOTES.md)  
- Java modules: [java/README.md](java/README.md)  
- Java developer notes: [java/NOTES.md](java/NOTES.md)
