import argparse
import warnings


def print_help_all(
    parser: argparse.ArgumentParser | None = None,
    subparsers: argparse._SubParsersAction | None = None,
) -> bool:
    """Print top-level help along with help for every registered subcommand.

    When ``subparsers`` is omitted the function falls back to inspecting the
    parser's private ``_actions`` list. That path emits a ``RuntimeWarning`` so
    callers are reminded to keep the ``_SubParsersAction`` returned by
    ``add_subparsers()``.

    Returns:
        ``True`` when help was rendered; ``False`` if no parser was supplied.
    """
    if parser is None:
        return False

    parser.print_help()

    target = subparsers
    if target is None:
        warnings.warn(
            "print_help_all is inspecting argparse internals; pass the "
            "_SubParsersAction returned by add_subparsers() to avoid this.",
            RuntimeWarning,
            stacklevel=2,
        )
        for action in getattr(parser, "_actions", []):
            if isinstance(action, argparse._SubParsersAction):
                target = action
                break
        else:
            return True  # no subparsers defined

    for name, subparser in target.choices.items():
        print()
        print(f"== {name}")
        subparser.print_help()

    return True
