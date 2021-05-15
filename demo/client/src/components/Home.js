import React from 'react'
import axios from 'axios'
import Menu from './Menu'

class Home extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            response: '',
            username: '',
            welcome: '',
        }
    }

    // handle the username
    handleUsername = e => {
        this.setState({
            [e.target.name]: e.target.value,
        })
    }
    
    confirmUsername = e => {
        e.preventDefault()
        console.log("Input: " + this.state.username)

        const req = {message: '6', username: this.state.username}

        axios
            .post('http://localhost:9000/username', req)
            .then(res => {
                if (res.data.response === '1') {
                    this.setState({ welcome: 'Hello ' + this.state.username }) 
                } else {
                    this.setState({ welcome: 'Username existed. Please choose another one!!'})
                }
            })
            .catch(err => console.error(err))
    }

    render() {
        return(
            <>
                <div className='header'>
                    <p>ONLINE OTHELLO</p>
                    <input type='text' name='username' onChange={this.handleUsername} /><br />
                    <button onClick={this.confirmUsername}>Confirm</button>
                    <h4>{this.state.welcome}</h4>
                </div>

                <div className='menu'>
                  
                    <Menu username={this.state.username} />
                </div>
            </>  
        )
    }
}

export default Home