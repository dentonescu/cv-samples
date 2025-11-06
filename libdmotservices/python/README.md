# libdmotservices/python

Python helpers that complement the multi-language `libdmotservices` toolkit. The package currently exposes:

- `libdmotservices.args.print_help_all` — prints a parser’s standard help plus every subcommand’s help text. It doubles as a developer aid (`--help-all`) in CLI entry points like [`pkixwebadm`](../../pkixwebadm/README.md) and accepts an explicit `_SubParsersAction` handle to avoid relying on argparse internals.
- `libdmotservices.string.get_resource_base_name` — extracts the right-most component of a POSIX or Windows path and is reused by the FastAPI static-file mount in pkixwebadm.

## Quick start

```bash
python3 -m pip install -e .
pytest
```

The editable install makes the utilities available to sibling projects inside this repository. The pytest suite exercises both helpers and asserts the warnings emitted when callers ignore the recommended `_SubParsersAction` argument.

## Layout

| Path | Purpose |
|------|---------|
| `libdmotservices/args.py` | `print_help_all` helper and related tests. |
| `libdmotservices/string.py` | Cross-platform path utilities. |
| `tests/test_args.py` | Captures the combined help output and the warning behaviour. |
| `tests/test_string.py` | Ensures path handling stays portable. |

Return to the [libdmotservices overview](../README.md) or the broader implementation plan in [NOTES.md](../NOTES.md).
