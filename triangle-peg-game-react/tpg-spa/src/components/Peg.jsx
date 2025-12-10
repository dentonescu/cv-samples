export default function Peg({ pos, onPosClick }) {
    return (
        <div className="peg" data-pos="{pos}" onClick={onPosClick}>
            {/* placeholder to prevent the peg from collapsing */}
            {' '}
        </div>
    );
}