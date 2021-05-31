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
                    <td><button id='join-btn' onClick={() => {this.joinRoom(id, player1)}}>Join</button></td>
                </tr>
            )
        })
    }

    joinRoom(id, player1) {
        // Send ("3" <roomdId> <username>)
        // Receive: (<board> <turn> <host>)
        const req = { message: '3', roomID: id, player2: this.props.username, player1: player1}

        axios
            .post('http://localhost:9001/join', req)
            .then(res => {
                const board = res.data.response.split(' ').slice(0,64)
                const turn = res.data.response.split(' ')[64]
                const host = player1
                //this.setState({response: res.data.response})
                ReactDOM.render(<Game username={this.props.username} board={board} turn={turn} player1={host} player2={req.player2} roomID={id}/>, document.getElementById('root'))
            }).catch(err => console.log(err))
    }

    render() {
        
        return (
            <>
                <div className='table'>
                    <h1 id='title'>ONLINE ROOMS - Username: {this.props.username} </h1>
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