import Hole from './Hole'
import Peg from './Peg'

const rows = [[1], [2, 3], [4, 5, 6], [7, 8, 9, 10], [11, 12, 13, 14, 15]];

export default function Board(props) {
    return (
        <div className="board">
            {rows.map((row, idx) => (
                <div className="row" key={idx}>
                    {row.map(pos => (
                        <Hole key={pos} pos={pos} />
                    ))}
                </div>
            ))}
        </div>
    );
}