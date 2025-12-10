import Hole from './Hole'
import Peg from './Peg'

export default function Position({ pos, isPegged, handleClick }) {

    return isPegged ?
        (<Peg pos={pos} onPosClick={() => handleClick(pos)} />)
        :
        (<Hole pos={pos} onPosClick={() => handleClick(pos)} />);
}