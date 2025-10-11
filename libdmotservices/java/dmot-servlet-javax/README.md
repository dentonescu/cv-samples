# libdmotservices-servlet-javax

Utilities for `javax.servlet` (Servlet 3.1) environments.
**Purpose:** keep legacy servlet helpers isolated from the dependency-light core module.

- **Java:** 17 (`--release 17`)
- **Servlet API:** `javax.servlet:javax.servlet-api:3.1.0` (scope: provided)
- **Intended use:** classic Tomcat / Java EE apps on `javax.*`

> Modern stacks (Spring Boot 3+, Jakarta EE 9+) use `jakarta.servlet`.
> This module is deliberately `javax` for compatibility with older deployments.

## Install

_Maven_
```xml
<dependency>
  <groupId>dev.dmotservices</groupId>
  <artifactId>libdmotservices-servlet-javax</artifactId>
  <version>1.0.0</version>
</dependency>
