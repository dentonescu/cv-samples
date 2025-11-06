import argparse
import contextlib
import io
import logging

import pytest

from libdmotservices import print_help_all

logger = logging.getLogger(__name__)


def build_mock_arg_parser() -> tuple[argparse.ArgumentParser, argparse._SubParsersAction]:
    parser = argparse.ArgumentParser(description="mock arg parser", prog="mock")
    parser.add_argument("--eat", action="store_true", help="Eat something")
    parser.add_argument("--drink", action="store_true", help="Drink something")
    subparsers_1 = parser.add_subparsers(dest="do_something", required=False)
    do_something = subparsers_1.add_parser("do", help="Do something")
    do_something.add_argument("--karate", action="store_true", help="Do karate")
    do_something.add_argument("--yoga", action="store_true", help="Do yoga")
    go_somewhere = subparsers_1.add_parser("go", help="Go somewhere")
    go_somewhere.add_argument("--beach", action="store_true", help="Go to the beach")
    go_somewhere.add_argument("--cinema", action="store_true", help="Go to the cinema")
    go_somewhere.add_argument("--mall", action="store_true", help="Go to the mall")
    return parser, subparsers_1


def test_print_help_all():
    mock_arg_parser, subparsers = build_mock_arg_parser()

    buffer = io.StringIO()
    with contextlib.redirect_stdout(buffer):
        assert print_help_all(mock_arg_parser, subparsers) is True

    output = buffer.getvalue()
    for keyword in (
        "Eat something",
        "Drink something",
        "Do something",
        "Go somewhere",
        "Do karate",
        "Go to the beach",
    ):
        assert keyword in output


def test_print_help_all_warns_when_subparsers_missing():
    mock_arg_parser, _ = build_mock_arg_parser()
    with pytest.warns(RuntimeWarning):
        assert print_help_all(mock_arg_parser) is True
