import axios from 'axios'
import React from 'react'
import ReactDOM from 'react-dom'
import Game from '../game/Game'

class Table extends React.Component {

    /* Test */
    constructor(props) {
        super(props)
        this.state = {
            roomID: '',
            username: '',
            player1: '',
            player2: '',
            response: '',
        }
    }

    renderTableData() {
        return this.props.data.map((room, index) => {
            const { id, player1, score1, score2, player2 } = room //destructuring
            return (
                <tr key={index}>
                    <td>{id}</td>
                    <td>{player1}</td>
                    <td>{score1} - {score2}</td>
                    <td>{player2}</td>
                    <td><button id='join-btn' onClick={() => {if (player2 === '#') {this.joinRoom(id, player1)} else { this.spectateGame(id)}}}>Join</button></td>
                </tr>
            )
        })
    }
    /* [FUNCTION] - Spectate a game */
    spectateGame(id) {
        const req = { message : '3', roomID: id, spectator: this.props.username }
        console.log("[SPECTATE]")
        console.log(req)
        axios.post('http://localhost:9002/spectate', req)
            .then(res => {
                const tmp = res.data.response.split(' ')
                const board = tmp.slice(0,64)
                const currentTurn = tmp[64]
                const host = tmp[65]
                const player2 = tmp[66]
                const score1 = tmp[67]
                const score2 = tmp[68]
                const turn = '3'
                ReactDOM.render(<Game username={this.props.username} board={board} currentTurn={currentTurn} turn={turn} host={host} player2={player2} roomID={id} score1={score1} score2={score2} />, document.getElementById('root'))
            }).catch(err => console.error(err))
    }

    /* [FUNCTION] - Join a game */
    joinRoom(id, player1) {
        // Send ("3" <roomdId> <username>)
        // Receive: (<board> <turn> <host>)
        const req = { message: '3', roomID: id, player2: this.props.username, player1: player1}

        axios
            .post('http://localhost:9002/join', req)
            .then(res => {
                const board = res.data.response.split(' ').slice(0,64)
                const turn = res.data.response.split(' ')[64]
                const host = player1
                const currentTurn = ''
                //this.setState({response: res.data.response})
                ReactDOM.render(<Game username={this.props.username} board={board} turn={turn} host={host} player2={req.player2} roomID={id} currentTurn={currentTurn} />, document.getElementById('root'))
            }).catch(err => console.log(err))
    }

    render() {
        
        return (
            <>
                <div className='table'>
                    <h1 id='title'>ONLINE ROOMS</h1>
                    <table id='rooms'>
                        <tbody>
                            <tr>
                                <th>ID</th>
                                <th>Player 1</th>
                                <th>Score</th>
                                <th>Player 2</th>
                                <th>Action</th>
                            </tr>
                            {this.renderTableData()}
                        </tbody>
                    </table>
                </div>

            </>
        )
    }
}

export default Table