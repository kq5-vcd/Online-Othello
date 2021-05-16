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
            response: '', 
            username: '',
            data: [],
            roomID: ''
        }
    }

    // load the current rooms
    loadRoom = () => {
    
        const req = { message: '1'}
        axios
            .post('http://localhost:9000/loadRoom', req)
            .then(res => {
                this.setState({response: res.data.response})
                this.setState({data: []})
                console.log(this.state.data)
                //console.log(this.state.response)
                const roomList = this.state.response.split(',')
                //console.log(roomList)
                var rooms = []
                console.log("Roomlist: " + roomList.length)
                for (var i = 0; i < roomList.length; i++) {
                    const room = roomList[i].split(' ')
                    const roomInfo = { id: room[0], player1: room[1], player2: '', score1: room[2], score2: room[4]}

                    this.setState({data: [...this.state.data, roomInfo]})
                }
                //console.log(rooms)

                //this.setState({data: [...this.state.data,rooms]})
                
            }).catch(err => console.error(err))
    }

    componentWillMount() {
        this.loadRoom()
    }

    showRoom() {
        return (
            <>
            Username: {this.props.username}
            <Table data={this.state.data} username={this.props.username}/>
            </>
        )
    }
    
    // user clicks the 'Create new room' button
    createNewRoom = () => {

        const req = {message: '2', username: this.props.username}

        console.log(req)
        
        axios
            .post('http://localhost:9000/create', req)
            .then(res => {
                console.log(this.state.response)
                console.log(res.data.response.split(' ')[64])
                this.setState({response: res.data.response, roomID: res.data.response.split(' ')[64]})
                ReactDOM.render(<Game username={this.props.username} board={this.state.response} roomID={this.state.roomID}/>, document.getElementById('root'))
                console.log(this.state.roomID)
                fetch('http://localhost:9000/ingame')
                    .then(res => res.text())
                    .then(res => {
                        console.log('Res: ' + res)
                        ReactDOM.render(<Game username={this.props.username} board={res} roomID={this.state.roomID}/>, document.getElementById('root'))        
                    })
                    .catch(err => console.error(err))
            })
            .catch(err => {
                console.error(err)})
            
    }

    render () {
        return(
            <>
                <div className='search'>
                    <p>ONLINE OTHELLO </p>
                    <input type='text' name='search' onChange={this.handleSearch} />
                    <button className='createRoom' onClick={this.createNewRoom}>Create New Room</button>
                </div>
                <div className='table'>
                    {this.showRoom()}
                    {/* <Table data={this.state.response} /> */}
                </div>
                
            </>
        )
    }
}

export default Multi