# Raw TCP Protocol (Draft)

## Request framing
```
APIKEY:<key>\n
COMMAND-LEN:<bytes>\n
\n
<command bytes>
```
- Connection: TCP, configurable port; server closes after one response.
- `APIKEY` is compared to the server's configured key.
- `COMMAND-LEN` is the byte length of the command payload (UTF-8 text).

## Response framing
```
EXIT-CODE:<int>\n
STDOUT-LEN:<bytes>\n
STDERR-LEN:<bytes>\n
\n
<stdout bytes><stderr bytes>
```
- `EXIT-CODE` is the child process exit code (0 for success, non-zero otherwise).
- Output sections are raw bytes; lengths guide parsing.

## Minimal flow
1) Client connects.
2) Client sends request frame.
3) Server validates API key; on failure, returns `EXIT-CODE:401` and empty bodies.
4) Server executes the command and responds with the framed result.
5) Server closes the connection.

## Example (short form)
```
APIKEY:SECRET\n
COMMAND-LEN:4\n
\n
ls -l
```

## Navigation
- [Docs home](README.md)
- [Security notes](security.md)
- [Project README](../README.md)
