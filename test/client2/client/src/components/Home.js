import React from 'react'
import axios from 'axios'
import Menu from './Menu'
import Popup from './game/Popup'

class Home extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            username: '', //
            isNameValid: false, // if isNameValid == true, enable mode buttons
            input: '',
            message: ''
        }
    }

    handleUsername = e =>  {
        this.setState({
            [e.target.name]: e.target.value,
        })
    }

    componentDidMount() {
        
        // Get username from server
        const req = { message: '0' }
        axios.post('http://localhost:9001/getName', req)
            .then(res => {
                const username = res.data.username
                console.log('Username: ' + username)

                if (username !== 'null') {
                    this.setState({ username: username, isNameValid: true })
                }
                
            }).catch(err => console.error(err))
    }

    submitUsername = e => {
        const username = this.state.input
        const req = { message: '0', username: username}

        axios
            .post('http://localhost:9001/username', req)
            .then(res => {
                if (res.data.status === '1') {
                    this.setState({username: username, isNameValid: true, showPopup: true, message: `Welcome ${username} to ONLINE OTHELLO!!`})
                } else {
                    this.setState({ showPopup: true, message: 'Name existed. Please try another one!!'})
                }
            })
    }

    render() {

        return (
            <>
                <header>
                    <center>ONLINE OTHELLO</center>
                </header>
                <div className="input-panel">
                    <div className="input-form">
                        <input type="text" placeholder="Enter your name" name="input" value={this.state.input} onChange={this.handleUsername} />
                        <button onClick={this.submitUsername}>Submit</button>
                    </div>
                    <div className="message">
                        {this.state.message}
                    </div>
                </div>

                <Menu username={this.state.username} isNameValid={!this.state.isNameValid} />
            </>
        )
    }
}

export default Home