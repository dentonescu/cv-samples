# Tests

Pytest coverage for the Python helpers shipped with `libdmotservices`.

- `test_args.py` captures the combined help text produced by `print_help_all` and asserts the warning emitted when the `_SubParsersAction` handle is omitted.
- `test_string.py` keeps the cross-platform path utility honest by checking both POSIX and Windows separators.

Run the suite with `python3 -m pytest`. Return to the [package README](../README.md) or the broader project notes in [../../NOTES.md](../../NOTES.md).
