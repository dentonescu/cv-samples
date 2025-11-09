# pkixwebadm.security.auth

Authentication contracts and backend implementations.

| Path | Purpose |
| --- | --- |
| `auth_manager.py` | Abstract base class that every backend implements. |
| `native/` | Active username/password backend that issues cookie sessions. |
| `oidc/` | Placeholder for the upcoming OpenID Connect integration. |

Each backend must return `Identity` objects and honour the cookie/session expectations documented in [NOTES.md](../../../NOTES.md). Continue into [`native/README.md`](native/README.md) for the currently active implementation.

