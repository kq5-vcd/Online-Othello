import React from 'react'
import ReactDOM from 'react-dom'
// import Single from './singlePlay/Single'
import Multi from './multi/Multi'


class Menu extends React.Component {
    constructor(props) {
        super(props)
        this.state = {
            response: '',
            username: ''
        }
    }

    // select multi play mode
    multiPlay = () => {
        ReactDOM.render(<Multi username={this.props.username} status={this.props.status} />, document.getElementById('root'))
    }

    render() {
        return (
            <div className='menu'>
                <div className='button'>
                    <button disabled={this.props.status} onClick={this.singlePlay}>Single Play</button><br />
                    <button disabled={this.props.status} onClick={this.multiPlay}>Multiplayer</button><br />
                    <button>How to Play</button><br />
                    <button>Exit</button>
                </div>
            </div>
        )
    }
}

export default Menu