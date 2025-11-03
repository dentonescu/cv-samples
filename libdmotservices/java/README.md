# libdmotservices (Java)

**libdmotservices-java** contains reusable Java utilities and legacy servlet helpers, built with a focus on clean APIs and minimal dependencies. It mirrors the structure of the C modules while demonstrating multi-language consistency.

## Project Summary
This module demonstrates my ability to:
- Write organized, dependency-light Java libraries.
- Maintain consistent build processes with Maven (and optional Ant orchestration).
- Design modular APIs for Java EE and modern JVM environments.

## Modules
| Module | Description |
|---------|--------------|
| [dmot-core](dmot-core/README.md) | Core utilities: I/O, time, URIs, regex, and collections. |
| [dmot-servlet-javax](dmot-servlet-javax/README.md) | Legacy `javax.servlet` helpers, isolated from Jakarta-specific code. |

## Build & Versioning
Ant is used as an orchestrator in this project. However, the individual sub-projects can be built directly with Maven.

```bash
ant -Dant=/usr/bin/ant -Dmvn=/usr/bin/mvn -Dproject.java.home=/usr/lib/jvm/java-17-openjdk-amd64
```
Alternatively, use the top-level `Makefile` to build all components.

**Java baseline:** 17 (compiled with `--release 17`)

Small, reusable Java utilities I’ve built and refined over time. The repository is organized as Maven modules but orchestrated by Ant for convenience (artifact collection, filesystem moves, and centralized `${revision}` propagation).

## What this demonstrates
- API-oriented design and modular Java development.
- Compatibility management between legacy and modern servlet APIs.
- Build orchestration and cross-language design discipline.

## Screenshot
![Java build screenshot](libdmotservices-java.png)

## Related

- Developer notes: [NOTES.md](NOTES.md)
- Explore [dmot-core](dmot-core/README.md) and [dmot-servlet-javax](dmot-servlet-javax/README.md)
- Back to [libdmotservices overview](../README.md)
