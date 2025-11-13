"""Command-line interface helpers for the pkixwebadm package.

The functions here create the CLI argument parser and launch the pkixwebadm web application.
"""

from __future__ import annotations

import argparse
import uvicorn
import sys

from pkixwebadm import get_settings
from libdmotservices import print_help_all
from typing import Sequence

CMD_SERVE = "serve"


def build_arg_parser() -> tuple[argparse.ArgumentParser, argparse._SubParsersAction]:
    """Create the CLI argument parser for configuring pkixwebadm.

    Returns:
        A tuple containing the configured ``ArgumentParser`` and the associated
        ``_SubParsersAction`` so callers can interact with the registered commands.
    """
    parser = argparse.ArgumentParser(
        description="pkixwebadm X.509 web administration tool"
    )
    parser.add_argument(
        "--help-all", action="store_true", help="Full help for all commands"
    )
    subparsers = parser.add_subparsers(dest="command", required=False)

    serve = subparsers.add_parser(CMD_SERVE, help="Run the pkixwebadm web server")
    serve.add_argument("--host", help="Bind host (overrides PKIXWA_HOST)")
    serve.add_argument("--port", type=int, help="Bind port (overrides PKIXWA_PORT)")
    serve.add_argument(
        "--auto-reload",
        action="store_true",
        help="Enable auto-reload even when dev mode is disabled",
    )
    serve.add_argument(
        "--no-auto-reload",
        action="store_true",
        help="Disable auto-reload even when dev mode is enabled",
    )
    return parser, subparsers


#
# pkixwebadm entry point
#


def main(argv: Sequence[str] | None = None) -> None:
    """Parse CLI arguments and start the pkixwebadm web application.

    Args:
        argv: Optional sequence of argument strings, used primarily for tests.
              When omitted, ``sys.argv`` is consumed.
    """
    parser, subparsers = build_arg_parser()
    raw_args = list(argv) if argv is not None else sys.argv[1:]
    if not raw_args:
        parser.error("A command is required.")
        return
    args = parser.parse_args(argv)
    if args.help_all:
        print_help_all(parser, subparsers)
    elif args.command == CMD_SERVE:
        settings = get_settings()
        host = args.host or settings.host
        port = args.port or settings.port

        # honour explicit CLI toggles, otherwise derive from settings.dev_mode
        if args.auto_reload:
            auto_reload = True
        elif args.no_auto_reload:
            auto_reload = False
        else:
            auto_reload = settings.dev_mode

        uvicorn.run(
            "pkixwebadm.app:create_app",
            factory=True,
            host=host,
            port=port,
            reload=auto_reload,
        )
