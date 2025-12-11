// game.js: Game logic


/*
 * Constants
 */

const BOARD_SIZE = 15;
const STATE_ALL_PEGS = 2 ** BOARD_SIZE - 1;

/*
 * Error messages
 */
const ERR_POS_REQUIRED = "pos is required";
const ERR_POS_FROM_REQUIRED = "from position is required";
const ERR_POS_OVER_REQUIRED = "over position is required";
const ERR_POS_TO_REQUIRED = "to position is required";
const ERR_POS_OUT_OF_BOUNDS = "pos is out of bounds";
const ERR_ROW_REQUIRED = "row is required";
const ERR_COL_REQUIRED = "col is required";
const ERR_BOARD_STATE_REQUIRED = "board state is required";
const ERR_PRECOMPUTED_STATES_REQUIRED = "the precomputed states are required";


/*
 * Triangular math
 */

function rowFromPos(pos) {
    return Math.ceil((Math.sqrt(8 * pos + 1) - 1) / 2);
}

function colFromPos(pos) {
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    return pos - ((rowFromPos(pos) - 1) * rowFromPos(pos)) / 2;
}

function posFromRowCol(row, col) {
    if (typeof row !== 'number') throw new Error(ERR_ROW_REQUIRED);
    if (typeof col !== 'number') throw new Error(ERR_COL_REQUIRED);
    return (row - 1) * row / 2 + col;
}

function isPosPegged(state, pos) {
    if (typeof state !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let p0 = pos - 1;
    return ((state >> p0) & 1);
}

function isRowColPegged(state, row, col) {
    if (typeof state !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (typeof row !== 'number') throw new Error(ERR_ROW_REQUIRED);
    if (typeof col !== 'number') throw new Error(ERR_COL_REQUIRED);
    return isPosPegged(state, posFromRowCol(row, col));
}

function getOverPos(fromPos, toPos) {
    if (null == fromPos || null == toPos) {
        return null;
    }
    if (typeof fromPos !== 'number' || typeof toPos !== 'number') {
        return null;
    }
    let fromRow = rowFromPos(fromPos);
    let fromCol = colFromPos(fromPos);
    let toRow = rowFromPos(toPos);
    let toCol = colFromPos(toPos);
    // must be invalid if we're jumping more than two rows or columns
    let deltaX = toCol - fromCol;
    let deltaY = toRow - fromRow;
    if (Math.abs(deltaY) > 2) {
        return null;
    }
    if (Math.abs(deltaX) > 2) {
        return null;
    }
    // axes checks
    const isValidAxis =
        (deltaX === 0 && Math.abs(deltaY) === 2) ||             // vertical along the triangle
        (deltaY === 0 && Math.abs(deltaX) === 2) ||             // horizontal
        (Math.abs(deltaX) === 2 && Math.abs(deltaY) === 2);     // diagonal along the triangle
    if (!isValidAxis) return null;
    // the peg jumped over is between the two
    let overRow = (fromRow + toRow) / 2;
    let overCol = (fromCol + toCol) / 2;
    let overPos = posFromRowCol(overRow, overCol);
    return overPos;
}

/*
 * Moves
 */

function getStateAfterJumping(boardState, states, fromPos, overPos, toPos) {
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (null == states) throw new Error(ERR_PRECOMPUTED_STATES_REQUIRED);
    if (typeof fromPos !== 'number') throw new Error(ERR_POS_FROM_REQUIRED);
    if (typeof overPos !== 'number') throw new Error(ERR_POS_OVER_REQUIRED);
    if (typeof toPos !== 'number') throw new Error(ERR_POS_TO_REQUIRED);
    if (states[boardState] && states[boardState].future_states) {
        let newState = getStateAfterSettingHole(boardState, fromPos);
        newState = getStateAfterSettingHole(newState, overPos);
        newState = getStateAfterSettingPeg(newState, toPos);
        let allowedFutureStates = states[boardState].future_states;
        if (allowedFutureStates.includes(newState)) {
            return newState;
        }
    }
    return boardState;
}

function getStateAfterSettingPeg(boardState, pos) {
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let newState = boardState;
    newState |= 1 << (pos - 1);
    return newState;
}

function getStateAfterSettingHole(boardState, pos) {
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let newState = boardState;
    newState &= ~(1 << (pos - 1));
    return newState;
}

function isJumpPossible(boardState, states, pegPos, holePos) {
    if (null == boardState || null == states || null == pegPos || null == holePos) {
        return false;
    }
    if (pegPos < 1 || pegPos > BOARD_SIZE) {
        return false;
    }
    if (holePos < 1 || holePos > BOARD_SIZE) {
        return false;
    }
    if (!isPosPegged(boardState, pegPos)) {
        return false;
    }
    if (isPosPegged(boardState, holePos)) {
        return false;
    }
    let fromPos = pegPos;
    let toPos = holePos;
    let overPos = getOverPos(fromPos, toPos);
    if (null == overPos)
        return false;
    let overRow = rowFromPos(overPos);
    let overCol = colFromPos(overPos);
    // another check bordering on the too cautious side
    if (overPos < 1 || overPos > BOARD_SIZE) {
        return false;
    }
    if (!isRowColPegged(boardState, overRow, overCol)) {
        return false;
    }
    // validate the new state against the known possible states
    let newState = getStateAfterJumping(boardState, states, fromPos, overPos, toPos);
    return (boardState != newState); // a jump isn't possible if the state hasn't changed
}


/*
 * Game logic
 */
const countPegs = (state) => state.toString(2).replace(/0/g, '').length; // 2 for binary

function getStatusMessage(boardState, states) {
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    let msg = "";
    let pegs = countPegs(boardState);
    if (pegs == BOARD_SIZE) {
        msg = "Remove a peg to begin the game.";
    } else if (pegs > 1) {
        msg = isGameOver(boardState, states) ?
            "GAME OVER. No more moves possible!" :
            "Select a peg, then a hole to jump to.";
    }
    else if (pegs == 1) {
        msg = "YOU WIN!";
    }
    return msg;
}

function getStateAfterHandlingPos(boardState, states, pos, chosenPeg, chosenHole) {
    if (null == boardState || null == states || null == pos) {
        // ignore an invalid game state
        return boardState;
    }
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let overPos = getOverPos(chosenPeg, chosenHole);
    let pegs = countPegs(boardState);
    if (pegs == BOARD_SIZE) {
        // remove a peg since the board is full
        return getStateAfterSettingHole(boardState, pos);
    } else if (pegs > 1) {
        if (null != chosenPeg && null != chosenHole) {
            if (null != overPos) {
                boardState = getStateAfterJumping(boardState, states, chosenPeg, overPos, chosenHole);
            }
        }
    }
    return boardState;
}

function isGameOver(boardState, states) {
    if (typeof boardState !== 'number') throw new Error(ERR_BOARD_STATE_REQUIRED);
    if (null == states || null == states[boardState] || null == states[boardState].future_states)
        throw new Error(ERR_PRECOMPUTED_STATES_REQUIRED);
    return states[boardState].future_states.length < 1; // game over = no future moves
}


/*
 * Exports
 */

export {
    BOARD_SIZE,
    STATE_ALL_PEGS,
    colFromPos,
    countPegs,
    getStatusMessage,
    posFromRowCol,
    rowFromPos,
    getOverPos,
    getStateAfterHandlingPos,
    getStateAfterJumping,
    getStateAfterSettingHole,
    getStateAfterSettingPeg,
    isGameOver,
    isJumpPossible,
    isPosPegged,
    isRowColPegged,
};
