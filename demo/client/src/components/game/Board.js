import React from 'react'
import Square from './Square'

const Board = ({ squares, onClick }) => {

    return (
        <div className="board">
            {squares.map( (square, i) => (
                <Square value={square} key={i} onClick={ () => { if (squares[i] == '-1') onClick(i) }} />
            ))}
        </div>
    )
    
}
// class Board extends React.Component {

//     constructor(props) {
//         super(props)

//     }

//     render() {
//         return(
//             <div className='board'>
//                 {this.props.squares.map( (square, i) => (
//                     <Square value={square} key={i} onClick={ () => onClick(i)} />
//                 ))}
//             </div>
//         )
//     }
// }

export default Board