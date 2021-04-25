import React from 'react'
import ReactDOM from 'react-dom'
import Single from './singlePlay/Single'
import Multi from './multiPlay/Multi'

const Menu = () => {

    const singlePlay = () => {
        console.log("You choose to play single mode!")
        ReactDOM.render(<Single />, document.getElementById('root'))
    }

    const multiPlay = () => {
        console.log("You choose to play multiplayer mode")
        ReactDOM.render(<Multi />, document.getElementById('root'))
    }

    const howToPlay = () => {
        console.log("You choose to see how to play")
    }

    return (
        <div className="menu">
            <button onClick={singlePlay}>Single Player</button><br />
            <button onClick={multiPlay}>Multiplayer</button><br />
            <button onClick={howToPlay}>How to play</button><br />
            <button>Exit</button><br/>
        </div>
    )
}



export default Menu