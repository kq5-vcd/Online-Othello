import axios from 'axios'
import React from 'react'
import ReactDOM from 'react-dom'
import Game from '../game/Game'
import Home from '../Home'

class Single extends React.Component {
    constructor(props) {
        super(props)
    }

    chooseEasy() {
        const req = { message: '9', difficulty: '1'}

        axios.post('http://localhost:9000/bot', req)
        .then(res => {
            const board = res.data.response.split(' ').slice(0,64)
            const turn = res.data.response.split(' ')[64]
            const roomID = '0'
            const bot = 'DUY'

            ReactDOM.render(<Game username={this.props.username} roomID={roomID} host={this.props.username} player2={bot} turn={turn} board={board} />, document.getElementById('root'))
        }).catch(err => console.error(err))
    }

    chooseMedium() {
        const req = { message: '9', difficulty: '1'}

        axios.post('http://localhost:9000/bot', req)
        .then(res => {
            const board = res.data.response.split(' ').slice(0,64)
            const turn = res.data.response.split(' ')[64]
            const bot = 'DUY'
            const roomID = '0'
            ReactDOM.render(<Game username={this.props.username} roomID={roomID} host={this.props.username} player2={bot} turn={turn} board={board} />, document.getElementById('root'))
        }).catch(err => console.error(err))
    }

    chooseHard() {
        const req = { message: '9', difficulty: '1'}

        axios.post('http://localhost:9000/bot', req)
        .then(res => {
            const board = res.data.response.split(' ').slice(0,64)
            const turn = res.data.response.split(' ')[64]
            const bot = 'DUY'
            const roomID = '0'
            ReactDOM.render(<Game username={this.props.username} roomID={roomID} host={this.props.username} player2={bot} turn={turn} board={board} />, document.getElementById('root'))
        }).catch(err => console.error(err))
    }

    backToMenu = () => {
        ReactDOM.render(<Home />, document.getElementById('root'))
    }

    render() {
        return (
            <>
                <header>
                    <center>ONLINE OTHELLO</center>
                </header>

                <div className="single-menu">
                    <button onClick={ () => this.chooseEasy()}>Easy</button>
                    <button onClick={ () => this.chooseMedium()}>Medium</button>
                    <button onClick={ () => this.chooseHard()}>Hard</button>
                    <button onClick={ () => this.backToMenu()}>Back to menu</button>
                </div>

            </>
        )
    }
}

export default Single