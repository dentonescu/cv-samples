export default function Hole({ pos, onPosClick }) {
    return (
        <div className="hole" data-pos="{pos}" onClick={onPosClick}>
            {/* placeholder to prevent the hole from collapsing */}
            {' '}
        </div>
    );
}