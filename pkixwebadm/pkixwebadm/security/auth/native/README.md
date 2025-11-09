# pkixwebadm.security.auth.native

Native (username/password) authentication backend.

| File | Purpose |
| --- | --- |
| `NativeAuthManager.py` | Implements the `AuthManager` contract, validates credentials, and issues/tears down cookie-backed sessions. |
| `context.py` | Lightweight dependency container (settings, repositories, session store, password verifier). |
| `crypto.py` | bcrypt helpers that respect the configured round count. |

Repositories and session stores are supplied via the `NativeAuthContext` protocol, so tests can pass fakes and production can supply database-backed implementations. The manager is framework-agnostic—callers inject FastAPI request/response objects when running inside the web tier. Return to the [security overview](../../README.md) or proceed to the future `oidc` backend once it materialises.

