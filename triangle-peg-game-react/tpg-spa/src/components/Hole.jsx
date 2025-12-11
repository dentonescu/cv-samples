import { isJumpPossible } from "../logic/game";

export default function Hole({ boardState, states, pos, chosenPeg, onPosClick }) {
    let possibleMove = isJumpPossible(boardState, states, chosenPeg, pos) ? "possible-move" : "";
    return (
        <div className={`hole ${possibleMove}`} data-pos={pos} onClick={onPosClick}>
            {/* placeholder to prevent the hole from collapsing */}
            {' '}
        </div>
    );
}
