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
const ERR_POS_OUT_OF_BOUNDS = "pos is out of bounds";
const ERR_ROW_REQUIRED = "row is required";
const ERR_COL_REQUIRED = "col is required";
const ERR_STATE_REQUIRED = "state is required";


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
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let p0 = pos - 1;
    return ((state >> p0) & 1);
}

function isRowColPegged(state, row, col) {
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    if (typeof row !== 'number') throw new Error(ERR_ROW_REQUIRED);
    if (typeof col !== 'number') throw new Error(ERR_COL_REQUIRED);
    return isPosPegged(state, posFromRowCol(row, col));
}

/*
 * Moves
 */

function getStateAfterSettingPeg(state, pos) {
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let newState = state;
    newState |= 1 << (pos - 1);
    return newState;
}

function getStateAfterSettingHole(state, pos) {
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let newState = state;
    newState &= ~(1 << (pos - 1));
    return newState;
}


/*
 * Game logic
 */
const countPegs = (state) => state.toString(2).replace(/0/g, '').length; // 2 for binary

function getStatusMessage(state) {
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    let msg = "";
    let pegs = countPegs(state);
    if (pegs == BOARD_SIZE) {
        msg = "Remove a peg to begin the game."
    } else if (pegs == 1) {
        msg = "YOU WIN!"
    }
    return msg;
}

function getStateAfterHandlingPos(state, pos) {
    if (typeof state !== 'number') throw new Error(ERR_STATE_REQUIRED);
    if (typeof pos !== 'number') throw new Error(ERR_POS_REQUIRED);
    if (pos < 1 || pos > BOARD_SIZE) throw new RangeError(ERR_POS_OUT_OF_BOUNDS);
    let pegs = countPegs(state);
    if (pegs == BOARD_SIZE) {
        // remove a peg since the board is full
        return getStateAfterSettingHole(state, pos);
    }
    return state;
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
    isPosPegged,
    isRowColPegged,
    posFromRowCol,
    rowFromPos,
    getStateAfterHandlingPos,
    getStateAfterSettingHole,
    getStateAfterSettingPeg,
};