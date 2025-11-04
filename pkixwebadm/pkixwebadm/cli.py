"""Command-line interface helpers for the pkixwebadm package.

The functions here create the CLI argument parser and launch the pkixwebadm web application.
"""

from __future__ import annotations

import argparse
from typing import Sequence

from . import VERSION

def build_arg_parser() -> argparse.ArgumentParser:
    """Create the CLI argument parser for configuring pkixwebadm.

    Returns:
        An ``ArgumentParser`` preloaded with pkixwebadm-specific options.
    """
    parser = argparse.ArgumentParser(description="pkixwebadm X.509 web administration tool")
    return parser

#
# pkixwebadm entry point
#

def main(argv: Sequence[str] | None = None) -> None:
    """Parse CLI arguments and start the pkixwebadm web application.

    Args:
        argv: Optional sequence of argument strings, used primarily for tests.
              When omitted, ``sys.argv`` is consumed.
    """
    parser = build_arg_parser()
    args = parser.parse_args(argv)

