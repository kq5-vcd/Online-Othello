import React from 'react'
import axios from 'axios'
import ReactDOM from 'react-dom'
// import Single from './singlePlay/Single'
import Multi from './multi/Multi'


class Menu extends React.Component {

    constructor(props) {
        super(props)
        this.state = { 
            response : '',
            username: ''
        }
    }

    // select multi play mode
    multiPlay = () => {
        ReactDOM.render(<Multi username={this.props.username}/>, document.getElementById('root'))
    }
    
    render() {
        return(
            <div className='menu'>
                <button>Single Play</button><br />
                <button onClick={this.multiPlay}>Multiplayer</button><br />
                <button>How to Play</button><br />
                <button>Exit</button>
            </div>
        )
    }
}

export default Menu