# pkixwebadm.security

Security primitives shared across pkixwebadm.

| Path | Purpose |
| --- | --- |
| `credentials.py` | Pydantic model describing inbound authentication attempts. |
| `identity.py` | Immutable representation of authenticated users plus helper methods. |
| `auth/` | Authentication backends (native and planned OIDC) built on the shared contracts. |

These modules are imported by the FastAPI dependencies and CLI utilities; keep them framework-agnostic when possible. Continue navigation via the [auth README](auth/README.md) or jump back to the [package overview](../README.md).

