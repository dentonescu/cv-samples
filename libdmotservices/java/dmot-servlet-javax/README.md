# libdmotservices-servlet-javax

Utilities for `javax.servlet` environments (Servlet 3.1). This module isolates legacy servlet code for compatibility with older deployments.

## Project Summary
This submodule demonstrates my ability to:
- Maintain and modernize legacy systems gracefully.
- Manage modularity across evolving API namespaces (`javax.*` vs `jakarta.*`).
- Build lightweight support libraries for enterprise environments.

## Technical Summary
- **Java version:** 17 (`--release 17`)
- **Servlet API:** `javax.servlet:javax.servlet-api:3.1.0` (scope: provided)
- **Intended use:** legacy Tomcat / Java EE 7 environments

## What this demonstrates
- Enterprise Java familiarity and backward compatibility strategies.
- Awareness of Java EE to Jakarta EE transitions.
- Consistent, versioned API support across environments.

## Install

_Maven_
```xml
<dependency>
  <groupId>dev.dmotservices</groupId>
  <artifactId>libdmotservices-servlet-javax</artifactId>
  <version>0.1.0</version>
</dependency>
```

## Related

- Back to [libdmotservices (Java)](../README.md)
- Modern utilities live in [dmot-core](../dmot-core/README.md)
