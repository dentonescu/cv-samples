import Position from './Position'
import { isPosPegged, getStateAfterHandlingPos } from '../logic/game';
import { loadStates } from '../logic/states';

const rows = [[1], [2, 3], [4, 5, 6], [7, 8, 9, 10], [11, 12, 13, 14, 15]];

export default function Board({ boardState, setBoardState }) {

    function handleClick(pos) {
        let newState = getStateAfterHandlingPos(boardState, pos);
        setBoardState(newState);
    }

    return (
        <div className="board">
            {rows.map((row, idx) => (
                <div className="row" key={idx}>
                    {row.map(pos => (
                        <Position
                            key={pos}
                            pos={pos}
                            isPegged={isPosPegged(boardState, pos)}
                            handleClick={handleClick} />
                    ))}
                </div>
            ))}
        </div>
    );
}