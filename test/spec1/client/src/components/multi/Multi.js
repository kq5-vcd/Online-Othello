import axios from 'axios'
import React from 'react'
import ReactDOM from 'react-dom'
import Game from '../game/Game'
import Home from '../Home'
import Table from './Table'

class Multi extends React.Component {
    constructor(props) {
        super(props)
        this.state = { 
            rooms: '',
            response: '', 
            username: this.props.username,
            data: [],
            roomID: ''
        }
    }

    // load the current rooms
    loadRoom = () => {
    
        const req = { message: '1'}
        axios
            .post('http://localhost:9002/loadRoom', req)
            .then(res => {
                if (res.data.rooms != 'empty') {
                
                    const rooms = res.data.rooms.split(',')
                    console.log(rooms)
                    for (var i = 0; i < rooms.length; i++) {
                        const room = rooms[i].split(' ')
                        const roomInfo = { id: room[0], player1: room[1], player2: room[3], score1: room[2], score2: room[4]}
                        this.setState({data: [...this.state.data, roomInfo]})
                    }
                }
            }).catch(err => console.error(err))
    }

    componentDidMount() {
        this.loadRoom()
    }

    showRoom() {
        return (
            <Table data={this.state.data} username={this.props.username}/>
        )
    }
    
    // user clicks the 'Create new room' button
    createNewRoom = () => {
        // SEND: ("2" <username>)
        // RECEIVE: (<board> <roomId> 1)
        const req = {message: '2', username: this.props.username}
        
        axios
            .post('http://localhost:9002/create', req)
            .then(res => {
            
                // Receive: (<board> <room_id> <turn>)
                const tmp = res.data.response.split(' ')
                const board = tmp.slice(0,64)
                const roomID = tmp[64]
                const turn = tmp[65]
                const host = this.props.username
                const currentTurn = ''
                const isHost = true
                console.log('[CREATE] - ' + res.data.response)
                ReactDOM.render(<Game username={this.props.username} isHost={isHost} host={host} board={board} roomID={roomID} turn={turn} currentTurn={currentTurn} />, document.getElementById('root'))
            }).catch(err => {console.error(err)})
    }

    // user clicks back to menu
    backToMenu = () => {
        ReactDOM.render(<Home />, document.getElementById('root'))
    }

    render () {
        return(
            <>
                <header>
                    <button onClick={() => this.backToMenu()}>Back to menu</button>
                    <center>ONLINE OTHELLO</center>
                </header>
                <div className='search-panel'>
                    <div className="search-form">
                        <input type="text" name="search" placeholder="Search room" onChange={this.handleSearch} />
                        <button className="search-btn" onClick={() => this.loadRoom() }>Search</button>
                        <button className="create-btn" onClick={() => this.createNewRoom()}>Create</button>
                    </div>
                </div>
                <div className='table'>
                    {this.showRoom()}
                </div>
                
            </>
        )
    }
}

export default Multi