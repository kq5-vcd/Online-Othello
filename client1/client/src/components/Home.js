import React from 'react'
import axios from 'axios'
import Menu from './Menu'

class Home extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            username: '',
            status: '0',
            input: '',
        }
    }

    handleUsername = e =>  {
        this.setState({
            [e.target.name]: e.target.value,
        })
    }

    componentDidMount() {
        if (this.props.username !== undefined) {
            this.setState({ username: this.props.username, status: this.props.status, input: this.props.username})
        }
    }

    submitUsername = e => {
        const username = this.state.input
        
        const req = { message: '0', username: username}

        axios
            .post('http://localhost:9000/username', req)
            .then(res => {
                if (res.data.status === '1') {
                    this.setState({username: username, status: false})
                    console.log('Available')
                } else {
                    console.log('Try another one')
                }
            })
    }

    render() {
        return (
            <>
                <div className='logo'>
                    <p>ONLINE OTHELLO</p>
                </div>

                <div className='header'>
                    <input type='text' name='input' value={this.state.input} onChange={this.handleUsername} /><br />
                    <button disabled={!this.state.status} onClick={this.submitUsername}>Confirm</button>
                    
                </div>

                <div className='menu'>
                    <Menu username={this.state.username} status={this.state.status} />
                </div>
            </>
        )
    }
}

export default Home