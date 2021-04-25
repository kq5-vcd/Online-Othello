import React from 'react'

const Player = () => {
    return (
        <div className='header'>
            <h2>ONLINE OTHELLO</h2>
            <div class="userName">
                <input type="text" placeholder="Enter your name: " />
                <button>Confirm</button>
            </div>
        </div>
    )
}

export default Player;