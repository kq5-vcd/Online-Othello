import React from 'react'
import Square from './Square'

class Board extends React.Component {
    
    constructor(props) {
        super(props)
    }

    render() {
        return (
            <div className='board'>
                
                {this.props.squares.map( (square, i) => (
                    <Square value={square} key={i} index={i}  squares={this.props.squares} roomID={this.props.roomID}/>
                ))}
            </div>
        )
    }
}

export default Board