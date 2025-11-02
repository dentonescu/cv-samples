"""Module entry point for `python -m slideshow_server`."""

from . import main


def _run() -> None:
    main()


if __name__ == "__main__":
    _run()
