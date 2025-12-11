export default function Peg({ pos, chosenPeg, onPosClick }) {
    let pegSelected = (pos === chosenPeg) ? "peg-selected" : "";
    return (
        <div className={`peg ${pegSelected}`} data-pos={pos} onClick={onPosClick}>
            {/* placeholder to prevent the peg from collapsing */}
            {' '}
        </div>
    );
}
