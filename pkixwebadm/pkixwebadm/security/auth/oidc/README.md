# pkixwebadm.security.auth.oidc

Placeholder for the OpenID Connect authentication backend. The FastAPI dependencies already anticipate an `AuthManager`, so this folder will house the token validation logic, discovery helpers, and session bridging once we schedule the federated-auth slice (see [NOTES.md](../../../../NOTES.md)).

Until then, the native backend (`../native/`) remains the active implementation.

