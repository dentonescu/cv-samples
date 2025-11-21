# Security Notes

- Demo only. Do not expose this service outside a closed, trusted network.
- API keys are plaintext over TCP; there is no TLS, no replay protection, and no rate limiting.
- The server executes arbitrary commands. Use disposable VMs or lab machines.
- Prefer running as a non-privileged user in a sandboxed environment.
- Rotate or randomize API keys for each demo session; do not reuse secrets.

## Navigation
- [Docs home](README.md)
- [Protocol](protocol.md)
- [Project README](../README.md)
