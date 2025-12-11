import { useEffect, useState } from 'react';
import Position from './Position'
import { isPosPegged, getStateAfterHandlingPos } from '../logic/game';

const rows = [[1], [2, 3], [4, 5, 6], [7, 8, 9, 10], [11, 12, 13, 14, 15]];

export default function Board({ boardState, states, setBoardState }) {

    const [boardPos, setBoardPos] = useState(null);
    const [chosenHole, setChosenHole] = useState(null);
    const [chosenPeg, setChosenPeg] = useState(null);

    function handleClick(pos) {
        setBoardPos(pos);
        if (isPosPegged(boardState, pos)) {
            setChosenPeg(pos);
        } else {
            setChosenHole(pos);
        }
    }

    useEffect(() => {
        const newState = getStateAfterHandlingPos(boardState, states, boardPos, chosenPeg, chosenHole);
        setBoardState(newState);
    }, [boardPos, chosenPeg, chosenHole, boardState, states, setBoardState]);

    return (
        <div className="board">
            {rows.map((row, idx) => (
                <div className="row" key={idx}>
                    {row.map(pos => (
                        <Position
                            key={pos}
                            boardState={boardState}
                            states={states}
                            pos={pos}
                            chosenPeg={chosenPeg}
                            handleClick={handleClick} />
                    ))}
                </div>
            ))}
        </div>
    );
}
