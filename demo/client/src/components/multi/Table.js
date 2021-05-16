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
            const { id, player1, player2, score1, score2 } = room //destructuring
            return (
                <tr key={index}>
                    <td>{id}</td>
                    <td>{player1}</td>
                    <td>{score1} - {score2}</td>
                    <td>{player2}</td>
                    <td><button id='join-btn' onClick={() => {this.joinRoom(id)}}>Join</button></td>
                </tr>
            )
        })
    }

    joinRoom(id) {
        const req = { message: '3', roomID: id, player2: this.props.username}

        console.log(req)
        axios
            .post('http://localhost:9001/join', req)
            .then(res => {
                console.log(res.data.response)
                this.setState({response: res.data.response})
                ReactDOM.render(<Game username={this.props.username} board={this.state.response} roomID={id} />, document.getElementById('root'))

                fetch('http://localhost:9001/getMove')
                    .then(res => res.text())
                    .then(res => {
                        ReactDOM.render(<Game username={this.props.username} board={res} roomID={id} />, document.getElementById('root'))

                    }).catch(err => console.error(err))

            }).catch(err => console.log(err))
        
        console.log("CHECK")
    }
 

    // renderTableHeader() {
    //     let header = Object.keys(this.state.students[0])
    //     return header.map((key, index) => {
    //         return <th key={index}>{key.toUpperCase()}</th>
    //     })
    // }
    

    render() {
        
        return (
            <div>
                <h1 id='title'>React Dynamic Table</h1>
                <table id='students'>
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
        )
    }
}

export default Table