#!/usr/bin/env python3
import argparse
import json
import math
from dataclasses import dataclass
from typing import Sequence

#
# Constants
#
BOARD_SIZE = 15


#
# Arguments
#


def build_arg_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="triangle-peg-game-react precompute utility"
    )
    parser.add_argument(
        "--export-json",
        type=str,
        metavar="JSON_FILE",
        help="Exports the future states and metadata as JSON",
    )
    parser.add_argument(
        "--show-board",
        type=int,
        metavar="STATE_INTEGER",
        help="Displays an ASCII board for the given state",
    )
    parser.add_argument(
        "--show-future-states",
        type=int,
        metavar="STATE_INTEGER",
        help="Displays the possible futures states for the given state",
    )
    return parser


#
# Triangular math
#


def row_from_pos(p: int) -> int:
    return math.ceil((math.sqrt(8 * p + 1) - 1) / 2)


def col_from_pos(p: int) -> int:
    r = row_from_pos(p)
    return p - ((r - 1) * r) // 2


def pos_from_row_col(r: int, c: int) -> int:
    return ((r - 1) * r) // 2 + c


def is_pos_pegged(state: int, pos: int) -> bool:
    p0 = pos - 1
    return (state >> p0) & 1


def is_row_col_pegged(state: int, row: int, col: int) -> bool:
    return is_pos_pegged(state, pos_from_row_col(row, col))


#
# Game logic
#


def move_and_get_new_state_if_possible(
    state: int, from_row: int, from_col: int, direction: str
) -> int | None:
    max_rows = row_from_pos(BOARD_SIZE)

    # bounds checks for the from position
    if from_row < 1 or from_row > max_rows:
        return None
    if from_col < 1 or from_col > from_row:
        return None

    #
    # The only offsets that keep us on valid holes are the three axes:
    # Horizontal: (Δr, Δc) = (0, ±2) → E/W
    # Down-right / up-left: (±2, ±2) → SE/NW
    # Down-left / up-right: (±2, 0) → S/N
    #
    match direction.upper():
        case "E":
            over_row = from_row
            over_col = from_col + 1
            to_row = from_row
            to_col = from_col + 2
        case "SE":
            over_row = from_row + 1
            over_col = from_col + 1
            to_row = from_row + 2
            to_col = from_col + 2
        case "S":
            over_row = from_row + 1
            over_col = from_col
            to_row = from_row + 2
            to_col = from_col
        case "SW":
            raise ValueError("A southwest move is invalid in this triangle.")
        case "W":
            over_row = from_row
            over_col = from_col - 1
            to_row = from_row
            to_col = from_col - 2
        case "NW":
            over_row = from_row - 1
            over_col = from_col - 1
            to_row = from_row - 2
            to_col = from_col - 2
        case "N":
            over_row = from_row - 1
            over_col = from_col
            to_row = from_row - 2
            to_col = from_col
        case "NE":
            raise ValueError("A northeast move is invalid in this triangle.")
        case _:
            raise ValueError("Unrecognized direction.")

    # bounds checks for the destination position
    if to_row < 1 or to_row > max_rows:
        return None
    if to_col < 1 or to_col > to_row:
        return None
    # bounds checks for the jumped over position (just to be on the safe side)
    if over_row < 1 or over_row > max_rows:
        return None
    if over_col < 1 or over_col > over_row:
        return None

    if (
        is_row_col_pegged(state, from_row, from_col)
        and is_row_col_pegged(state, over_row, over_col)
        and not is_row_col_pegged(state, to_row, to_col)
    ):

        pos_from = pos_from_row_col(from_row, from_col)
        pos_over = pos_from_row_col(over_row, over_col)
        pos_to = pos_from_row_col(to_row, to_col)
        new_state = state
        new_state &= ~(
            1 << (pos_from - 1)
        )  # remove the from peg by setting the bit to zero (0-based position)
        new_state &= ~(
            1 << (pos_over - 1)
        )  # remove the over peg by setting the bit to zero (0-based position)
        new_state |= 1 << (
            pos_to - 1
        )  # set the to peg by setting the bit to one (0-based position)
        return new_state

    return None


def find_possible_next_states(state: int) -> list[int]:
    possible_next_states = []
    max_rows = row_from_pos(BOARD_SIZE)
    for row in range(1, max_rows + 1):
        for col in range(1, row + 1):

            N = move_and_get_new_state_if_possible(state, row, col, "N")
            # No NE in this triangle
            E = move_and_get_new_state_if_possible(state, row, col, "E")
            SE = move_and_get_new_state_if_possible(state, row, col, "SE")
            S = move_and_get_new_state_if_possible(state, row, col, "S")
            W = move_and_get_new_state_if_possible(state, row, col, "W")
            # No SW in this triangle
            NW = move_and_get_new_state_if_possible(state, row, col, "NW")

            if N:
                possible_next_states.append(N)
            if E:
                possible_next_states.append(E)
            if SE:
                possible_next_states.append(SE)
            if S:
                possible_next_states.append(S)
            if W:
                possible_next_states.append(W)
            if NW:
                possible_next_states.append(NW)
    return possible_next_states


#
# UI
#


def state2ascii(state: int) -> str:
    max_rows = row_from_pos(BOARD_SIZE)
    max_cols = max_rows
    lines = [""] * max_rows
    for p in range(1, BOARD_SIZE + 1):
        r = row_from_pos(p)
        r0 = r - 1
        bit = is_pos_pegged(state, p)
        lines[r0] += "●" if bit else "○"
        c = col_from_pos(p)
        if c < max_cols:
            lines[r0] += " "
    for r0 in range(0, max_rows):
        lines[r0] = lines[r0].center(max_cols * 2)  # pad on both sides with spaces
    return "\n".join(lines)


def show_board(state: int) -> None:
    print(f"state: {state}")
    print(f"bitmap: {state:0{BOARD_SIZE}b}")
    print("board:")
    print(f"{state2ascii(state)}")


def show_future_states(state: int) -> None:
    print("[current position]")
    show_board(state)
    future_states = find_possible_next_states(state)
    for fs in future_states:
        print(f"\n[future state: {fs}]")
        show_board(fs)


#
# File I/O
#


@dataclass
class StateInfo:
    state: int
    peg_count: int
    is_winner: bool
    future_states: list[int]


def export_json(json_file: str) -> None:
    max_states = 2**BOARD_SIZE - 1
    states: list[StateInfo] = []
    for s in range(0, max_states + 1):
        states.append(
            StateInfo(
                state=s,
                peg_count=s.bit_count(),
                is_winner=(s.bit_count() == 1),
                future_states=find_possible_next_states(s),
            )
        )
    json_data = [vars(si) for si in states]
    with open(json_file, "w", encoding="utf-8") as f:
        json.dump(json_data, f, indent=2)


#
# Main entry point
#


def main(argv: Sequence[str] | None = None) -> None:
    parser = build_arg_parser()
    args = parser.parse_args(argv)

    if args.export_json:
        export_json(args.export_json)
    if args.show_board:
        show_board(args.show_board)
    if args.show_future_states:
        show_future_states(args.show_future_states)


if __name__ == "__main__":
    main()
