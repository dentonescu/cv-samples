import {
    BOARD_SIZE,
    countPegs,
    isGameOver,
    getStatusMessage,
} from '../logic/game.js'

export default function StatusBar({ boardState, states }) {
    const isEndOfGame = isGameOver(boardState, states);
    const pegs = countPegs(boardState ?? 0);
    let msg = getStatusMessage(boardState ?? 0, states);

    return (
        <>
            <div className="status-bar">
                <div className="status-pill">
                    <span className="status-label">Peg count</span>
                    <span className="status-value">{pegs}</span>
                </div>
                <div className="status-pill ghost">
                    <span className="status-label">Status</span>
                    <span className="status-value">{msg}</span>
                </div>
            </div>
            {isEndOfGame && (pegs != BOARD_SIZE)  && <div className="statusOverlay">{msg}</div>}
        </>
    );
}
