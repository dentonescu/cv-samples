import {
    countPegs,
    getStatusMessage,
} from '../logic/game.js'

export default function StatusBar({ boardState }) {
    const pegs = countPegs(boardState ?? 0);
    let msg = getStatusMessage(boardState ?? 0);

    return (
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
    );
}
