"""Module entry point for ``python -m pkixwebadm``.

Running the package as a module simply delegates to the CLI ``main`` helper so
the behaviour matches invoking the installed console script.
"""

from .cli import main


def _run() -> None:
    """Invoke the CLI main function.

    This indirection keeps ``if __name__ == "__main__"`` blocks compact and
    simplifies testing.
    """
    main()


if __name__ == "__main__":
    _run()
