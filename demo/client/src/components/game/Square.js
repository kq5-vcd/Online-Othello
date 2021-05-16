import React from 'react'
import axios from 'axios'
import ReactDOM from 'react-dom'
import Game from './Game'

class Square extends React.Component {

    constructor(props) {
        super(props)
    }

    handleClick(i) {
        // console.log(this.props.index)
        console.log('Position: ' + i)
        const move_row = parseInt(i/8)
        const move_col = i%8

        const req = { message: '4', roomID: this.props.roomID, move_row: move_row, move_col: move_col }
        console.log(req)
        axios
            .post('http://localhost:9000/move', req)
            .then(res => {
                    console.log(res.data.response)
                    ReactDOM.render(<Game username={this.props.username} board={res.data.response} roomID={this.props.roomID} />, document.getElementById('root'))
     
                        fetch('http://localhost:9000/getMove')
                                .then(res => res.text())
                                .then(res => {
                                    ReactDOM.render(<Game username={this.props.username} board={res} roomID={this.props.roomID} />, document.getElementById('root'))
                                })
                    
             }).catch(err => console.error(err))
    }

    render() {
        if (this.props.value === '-1') {
            return (
                <button className='squares' onClick={ () => this.handleClick(this.props.index)}>
                    <div className={'a' + this.props.value}></div>
                </button>
            )
        } else if (this.props.value === '1' || this.props.value === '2') {
            return (
                <button className='squares'>
                    <div className={'a' + this.props.value}></div>
                </button>
            )
        } else {
            return (
                <button className='squares'>
                </button>
            )
        }
    }
}

export default Square
