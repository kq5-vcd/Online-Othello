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
            .post('http://localhost:9001/loadRoom', req)
            .then(res => {
                if (res.data.rooms != 'empty') {
                
                    const rooms = res.data.rooms.split(',')

                    for (var i = 0; i < rooms.length; i++) {
                        const room = rooms[i].split(' ')
                        const roomInfo = { id: room[0], player1: room[1], player2: room[3], score1: room[2], score2: room[4]}
                        this.setState({data: [...this.state.data, roomInfo]})
                    }
                }
            }).catch(err => console.error(err))
    }

    componentDidMount() {
        //console.log(this.state)
        console.log('Username: ' + this.props.username)
        this.loadRoom()
        //this.showRoom()
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
            .post('http://localhost:9001/create', req)
            .then(res => {
            
                // Receive: (<board> <room_id> <turn>)
                const board = res.data.response.split(' ').slice(0,64)
                const roomID = res.data.response.split(' ')[64]
                const turn = res.data.response.split(' ')[65]
                const player1 = this.props.username
                const player2 = ''
                const start = false
                console.log('RoomID: ' + roomID)

                //this.setState({response: res.data.response})
                ReactDOM.render(<Game username={this.props.username} player1={player1} player2={player2}  board={board} roomID={roomID} turn={turn} start={start} />, document.getElementById('root'))
            }).catch(err => {console.error(err)})
    }

    // user clicks back to menu
    backToMenu = () => {
        ReactDOM.render(<Home />, document.getElementById('root'))
    }

    render () {
        return(
            <>
                <div className='logo'>
                    
                    <p>ONLINE OTHELLO </p>
                    {this.props.username}
                </div>
                <div className='toolbar'>
                    <div className='element'>
                        <input type='text' name='search' placeholder='Search room' onChange={this.handleSearch} />
                        <button className='createRoom' onClick={() => this.createNewRoom()}>Create New Room</button>
                        <button className='backToMenu' onClick={() => this.backToMenu() }>Back to Menu</button>
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