import Hole from './Hole'
import Peg from './Peg'
import { isPosPegged } from '../logic/game';

export default function Position({ boardState, states, pos, chosenPeg, handleClick }) {
    let isPegged = isPosPegged(boardState, pos);
    return isPegged ?
        (<Peg
            pos={pos} 
            chosenPeg={chosenPeg}
            onPosClick={() => handleClick(pos)} />)
        :
        (<Hole
            boardState={boardState}
            states={states}
            pos={pos}
            chosenPeg={chosenPeg}
            onPosClick={() => handleClick(pos)} />);
}
