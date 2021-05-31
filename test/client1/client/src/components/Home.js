import React from 'react'
import axios from 'axios'
import Menu from './Menu'
import Popup from './game/Popup'

class Home extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            username: '', //
            checkName: false,
            input: '',
            message: '',
            showPopup: false
        }
    }

    handleUsername = e =>  {
        this.setState({
            [e.target.name]: e.target.value,
        })
    }

    
    componentDidMount() {
        // if (this.props.username !== undefined) {
        //     this.setState({ username: this.props.username, status: this.props.status, input: this.props.username})
        // }
        const req = { message: '0' }
        axios.post('http://localhost:9000/getName', req)
            .then(res => {
                const username = res.data.username
                console.log('Username: ' + username)

                if (username !== 'null') {
                    this.setState({ username: username, checkName: true })
                }
                
            }).catch(err => console.error(err))
    }

    submitUsername = e => {
        const username = this.state.input
        const req = { message: '0', username: username}

        axios
            .post('http://localhost:9000/username', req)
            .then(res => {
                if (res.data.status === '1') {
                    this.setState({username: username, checkName: true, showPopup: true, message: 'Welcome to ONLINE OTHELLO!!'})
                } else {
                    this.setState({ showPopup: true, message: 'Name existed. Please try another one!!'})
                }
            })
    }

    render() {
        return (
            <>
                <div className="main">
                    <div className='logo'>
                        <p>ONLINE OTHELLO</p>
                    </div>
                </div>
                <div id="input-username">
                    <input type='text' name='input' value={this.state.input} onChange={this.handleUsername} />
                    <button onClick={this.submitUsername}>Confirm</button><br/>
                    
                    {/* <p trigger={this.state.showPopup}>{this.state.message}</p> */}
                </div>
            
                
                <div className='menu'>
                    <Menu username={this.state.username} checkName={!this.state.checkName} />
                </div>
                {/* <div className='logo'>
                    <p>ONLINE OTHELLO</p>
                </div>

                <div className='header'>
                    <input type='text' name='input' value={this.state.input} onChange={this.handleUsername} /><br />
                    <button onClick={this.submitUsername}>Confirm</button><br/>

                </div>

                <div className='menu'>
                    <Menu username={this.state.username} checkName={!this.state.checkName} />
                </div> */}
            </>
        )
    }
}

export default Home