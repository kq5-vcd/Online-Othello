import React from 'react'
import axios from 'axios'
import ReactDOM from 'react-dom'
import Popup from './Popup'
import Multi from '../multi/Multi'

class Game extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      message: '',
      roomID: this.props.roomID,
      board: this.props.board,
      turn: this.props.turn,
      score1: '2',
      score2: '2',
      player1: this.props.player1,
      player2: this.props.player2,
      trigger: false,
      start: this.props.start,
      status: '0'
    }
  }

  // get the opponent's move
  getMove() {
    fetch('http://localhost:9000/getMove')
              .then(res => res.text())
              .then(res => {
                const board = res.split(' ').slice(0,64)
                const score1 = res.split(' ')[67]
                const score2 = res.split(' ')[68]
                const turn = res.split(' ')[64]
                this.setState({ board: res.split(' ').slice(0,64), score1: score1 , score2: score2})

                if (turn !== this.state.turn && turn !== '-1') {
                  this.getMove()
                } else if (turn === '-1') {
                  this.setState({ trigger: true, status: '0'})
                }
              }).catch(err => console.error(err))
  }

  
  handleClick = (i) => {
    console.log('Position: ' + i)
    const move_row = parseInt(i/8)
    const move_col = i%8

    const req = { message: '5', roomID: this.props.roomID, move_row: move_row, move_col: move_col }
    console.log(req)
    axios
        .post('http://localhost:9000/move', req)
        .then(res => {
          const board = res.data.response.split(' ').slice(0,64)
          const score1 = res.data.response.split(' ')[67]
          const score2 = res.data.response.split(' ')[68]
          const turn = res.data.response.split(' ')[64]
          this.setState({ board: board, score1: score1, score2: score2 })

          console.log('Turn: ' + res.data.response.split(' ')[64] )
          
          if (turn !== this.state.turn && turn !== '-1')  {
            this.getMove()
          } else if (turn === '-1') {
            this.setState({ trigger: true, status: '0'})
            console.log('State: ' + this.state.status)
          }
        }).catch(err => console.error(err))
  }

  // 2 ways: 
  // - create new game: turn = 0 -> wait for /ingame
  // - join a game: turn = 2 -> wait for the opponent's move
  componentDidMount() {

    if (this.state.turn === '1') {
      
      fetch('http://localhost:9000/ingame')
        .then(res => res.text())
        .then(res => {
          this.setState({ player2: res.split(' ')[0], start: true})
        }).catch(err => console.error(err))
    } else if (this.state.turn === '2') {

      fetch('http://localhost:9000/ingame')
        .then(res => res.text())
        .then(res => {
          this.setState({ status: '1'})
          this.getMove()
        }).catch(err => console.error(err))
    }
  }

  // quit game 
  quitGame = () => {
    
    const req = {message: '6', roomID: this.props.roomID, state: this.state.status}
    console.log(req)
    axios
      .post('http://localhost:9000/quit', req)
      .then(res => {
        if (res.data.response === '0') {
          ReactDOM.render(<Multi username={this.props.username} />, document.getElementById('root'))
        }
        
      }).catch(err => console.error(err))
    
  }

  // host clicks start game
  startGame() {
    console.log('Start')
    const req = { message: '4', roomID: this.props.roomID}

    axios
      .post('http://localhost:9000/start', req)
      .then(res => {
        const board = res.data.response.split(' ').slice(0,64)
        const score1 = res.data.response.split(' ')[67]
        const score2 = res.data.response.split(' ')[68]
        this.setState({ board: board, score1: score1, score2: score2, start: false, status: '1' })
      }).catch(err => console.error(err))
  }

  playAgain() {
    const req = { message: '7', roomID: this.props.roomID, turn: this.state.turn }

    axios
      .post('http://localhost:9000/playAgain', req)
      .then(res => {
        console.log('New game')

        // get new board and hide the result panel
        const turn = res.data.response.split(' ')[64]
        const board = res.data.response.split(' ').slice(0,64)

        this.setState({ board: board, trigger: false })

        if (this.state.turn === '1') {
      
          fetch('http://localhost:9000/ingame')
            .then(res => res.text())
            .then(res => {
              this.setState({ player2: res.split(' ')[0], start: true})
            }).catch(err => console.error(err))
        } else if (this.state.turn === '2') {
    
          fetch('http://localhost:9000/ingame')
            .then(res => res.text())
            .then(res => {
              this.setState({ status: '1'})
              this.getMove()
            }).catch(err => console.error(err))

        }
      }).catch(err => console.error(err))
  }

  winner() {
    const score1 = this.state.score1
    const score2 = this.state.score2

    if (score1 < score2) {
      return (
        <> Winner: {this.state.player2}</>
      )
    } else if (score1 > score2) {
      return ( <> Winner: {this.state.player1} </>)
    } else {
      return ( 
        <> Draw </>
      )
    }
  }

  render() {
    return (
      <>
        <div className='room'>
          <Popup trigger={this.state.start}>
            <h3>Start Game?</h3>
            <button className='start-btn' onClick={() => this.startGame()}>Start game</button>
          </Popup>
          <Popup trigger={this.state.trigger}>
            <h3>{this.winner()}</h3>
            <button className='play-again' onClick={ () => this.playAgain() }>Play again</button>
            <button className='leave-room' onClick={() => this.quitGame() }>Leave room</button>
          </Popup>
          <button className='quit' onClick={() => this.quitGame()}>Quit</button>
         
          <div className='result'>
            <div className='player1'><p>{this.state.player1}</p></div>
            <div className='a1'></div>
            <div className='score'>
              <p>{this.state.score1} - {this.state.score2}</p>
            </div>
            <div className='a2'></div>
            <div className='player2'><p>{this.state.player2}</p></div>

            
          </div>
          <div className='game'>
            {this.state.board.map( (square, i) => {
              if (square === '-1') {
                  return (
                      <button className='squares' onClick={ () => this.handleClick(i)}>
                          <div className={'a' + square}></div>
                      </button>
                  )
              } else if (square === '1' || square === '2') {
                  return (
                      <button className='squares'>
                          <div className={'a' + square}></div>
                      </button>
                  )
              } else {
                  return (
                      <button className='squares'></button>
                  )
              }
            })}
          </div>
        </div>
      </>
    )
  }
}

export default Game