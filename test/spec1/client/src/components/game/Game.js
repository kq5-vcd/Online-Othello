import React from 'react'
import axios from 'axios'
import ReactDOM from 'react-dom'
import Popup from './Popup'
import Multi from '../multi/Multi'

class Game extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      /* room's info */
      roomID: this.props.roomID,
      board: this.props.board,
      turn: this.props.turn,
      host: this.props.host,
      player2: this.props.player2,
      score1: this.props.score1,
      score2: this.props.score2,
      currentTurn: this.props.currentTurn,
      isPlaying: '0',
      showStart: false,
      showResult: false,
      showNotification: false, 
      winByQuit: false,
      winner: '',
      loser: '',
      isHost: this.props.isHost,
      showResultForSpec: false
    }
  }

  /* [START GAME] */
  startGame() {

    const req = { message: '4', roomID: this.props.roomID }

    axios.post('http://localhost:9002/start', req)
      .then(res => {
        const tmp = res.data.response.split(' ')

        const board = tmp.slice(0,64)
        const turn = tmp[64]
        const player1 = tmp[65]
        const player2 = tmp[66]
        const score1 = tmp[67]
        const score2 = tmp[68]

        this.setState({ board: board, currentTurn: turn, player2: player2, score1: score1, score2: score2, showStart: false, isPlaying: '1', showResultForSpec: false })
        if (this.state.turn === '2') this.getMove()
      }).catch(err => console.error(err))
  }

  /* [GET MOVE] */
  getMove() {
  
    fetch('http://localhost:9002/getMove')
    .then(res => res.text())
    .then(res => {
        const tmp = res.split(' ')
        const board = tmp.slice(0,64)
        const turn = tmp[64]
        const player1 = tmp[65]
        const player2 = tmp[66]
        const score1 = tmp[67]
        const score2 = tmp[68]
        this.setState({ board: board, score1: score1 , score2: score2, currentTurn: turn})

        // if (turn !== '-1' && turn !== '-2') {
        //   this.setState({ showResultForSpec: false})
        // }

        switch(turn) {
          case '-1': // the game finished
            console.log('[GAME FINISHED]')
            console.log(score1 + ' ' +  score2)
            if (score1 > score2) {
              this.setState({ winner: this.state.host, loser: this.state.player2})
            } else if (score1 < score2) {
              this.setState({ winner: this.state.player2, loser: this.state.host})
            }
            if (this.state.turn === '3') {
              this.setState({ showResultForSpec: true })
              this.getMove()
            } else {
              this.setState({ showResult: true, isPlaying: '0'})
            }
            
            break;
          case '-2': // the component quits during game
            console.log('[OPPONENT QUIT]')

            if (this.state.isHost) {
              this.setState({ winner: this.state.host, loser: this.state.player2, player2: ''})
            } else {
              this.setState({ winner: this.state.player2, loser: this.state.host, host: this.props.username, isHost: true})
            }

            if (this.state.turn === '3') {
              this.setState({ showResultForSpec: true })
              this.getMove()
            } else {
              this.setState({ showResult: true, isPlaying: '0', winByQuit: true})
            }
            
            break;
          case '1': case '2': // the component keeps playing
            this.setState({ showResultForSpec: false })
            if (turn !== this.state.turn) this.getMove()
            break;
          default: 
            if (this.state.turn === '3') {
              this.setState({ showResultForSpec: false })
              this.getMove()
            }
            break;
        }
      }).catch(err => console.error(err))
  }

  spectatorQuit() {
  
    const req = { message: '8', roomID: this.props.roomID }
    axios.post('http://localhost:9002/spectateQuit', req)
      .then(res => {
        if (res.data.response === '0') {
          ReactDOM.render(<Multi username={this.props.username} />, document.getElementById('root'))
        }
      }).catch(err => console.error(err))
    
    
  }

  /* [QUIT GAME] */
  quitGame() {
    if (this.state.turn === '3') {
      this.spectatorQuit()
    } else {
      const req = { message: '6', roomID: this.props.roomID, isPlaying: this.state.isPlaying }
      axios.post('http://localhost:9002/quit', req)
        .then(res => {
          if (res.data.response === '0') {
            ReactDOM.render(<Multi username={this.props.username} />, document.getElementById('root'))
          }
        }).catch(err => console.error(err))
      }
    
  }

  /* [PLAY AGAIN] */
  playAgain() {
    const req = { message: '7', roomID: this.props.roomID }
    this.setState({ score1: '2', score2: '2', winByQuit: false, showResult: false, currentTurn: '', winner: '', loser: ''})

    axios.post('http://localhost:9002/playAgain', req)
      .then(res => {
        const tmp = res.data.response.split(' ')

        const board = tmp.slice(0,64)
        const turn = tmp[64]
        this.setState({ board: board, showResult: false})

        switch(turn) {
          case '0': // clicked 'Play again' first and wait for the other
            fetch('http://localhost:9002/ingame')
            .then(res => res.text())
            .then(res => {
              const turn = res.split(' ')[64]

              switch(turn) {
                case '0': // the opponent choose to quit
                  console.log('[MESSAGE] The opponent quit')
                  this.setState({ showNotification: true })
                  if (this.state.isHost) {
                    this.setState({ player2: '' })
                    this.waitForOpponent()
                  } else {
                    this.setState({ isHost: true, turn: '1', host: this.props.username, player2: '' })
                    this.waitForOpponent()
                  }
                  break
                case '1': case '2': // 
                  console.log('[MESSAGE] The opponent wants to play again and your turn is ' + turn)
                  this.setState({ turn: turn })
                  console.log('YOUR TURN: ' + this.state.turn)
                  if (this.state.isHost) {
                    this.setState({ showStart: true })
                  } else {
                    this.waitForSignalFromHost()
                  }
                  break
                default: break
              }
            }).catch(err => console.error(err))

            break
          case '1': case '2':
            console.log('[MESSAGE] The opponent wants to play again and your turn is ' + turn)
            this.setState({ turn: turn})
            console.log('YOUR TURN: ' + this.state.turn)
            if (this.state.isHost) {
              this.setState({ showStart: true })
            } else {
              this.waitForSignalFromHost()
            }
            break
          case '3':
            console.log('[MESSAGE] The opponent quit')

            this.setState({ isHost: true, host: this.props.username, player2: '', turn: '1', showNotification: true })
            this.waitForOpponent()
            break
          default: break
        }
      }).catch(err => console.error(err))

  }

  /* [HANDLE CLICK EVENT] */
  handleClick = (i) => {
    const move_row = parseInt(i/8)
    const move_col = i%8

    const req = { message: '5', roomID: this.props.roomID, move_row: move_row, move_col: move_col }
    
    axios.post('http://localhost:9002/move', req)
    .then(res => {
      const tmp = res.data.response.split(' ')
      const board = tmp.slice(0,64)
      const turn = tmp[64]
      const host = tmp[65]
      const player2 = tmp[66]
      const score1 = tmp[67]
      const score2 = tmp[68]

      this.setState({ board: board, score1: score1, score2: score2, currentTurn: turn })

      switch(turn) {
        case '-1': // game finished
          console.log(score1 + ' ' +  score2)
          if (score1 > score2) {
            this.setState({ winner: this.state.host, loser: this.state.player2})
          } else if (score1 < score2) {
            this.setState({ winner: this.state.player2, loser: this.state.host})
          }

          this.setState({ showResult: true, isPlaying: '0' })
          break
        case '-2': // 
          if (this.state.isHost) {
            this.setState({ winner: this.state.host, loser: this.state.player2, player2: ''})
          } else {
            this.setState({ winner: this.state.player2, loser: this.state.host, host: this.props.username, isHost: true})
          }
          this.setState({ showResult: true, isPlaying: '0', winByQuit: true })
          break
        case '1': case '2':
          if (turn !== this.state.turn) this.getMove()
          break
        default: break
      }
    }).catch(err => console.error(err))
  }

  /* [WAITING FOR AN OPPONENT TO JOIN] */
  waitForOpponent() {
    fetch('http://localhost:9002/ingame')
    .then(res => res.text())
    .then(res => {
      this.setState({ player2: res, showStart: true })
    }).catch(err => console.error(err))
  }

  /* [WAITING FOR THE SIGNAL FROM HOST] */
  waitForSignalFromHost() {
    fetch('http://localhost:9002/ingame')
    .then(res => res.text())
    .then(res => {
      const board = res.split(' ').slice(0,64)
      const turn = res.split(' ')[64]
      const score1 = res.split(' ')[67]
      const score2 = res.split(' ')[68]

      if (turn === '0') { // If the host quit game
        this.setState({ host: this.props.username, player2: '', turn: '1' })
        this.waitForOpponent()
      } else { // If the host starts game
        this.setState({ isPlaying: '1', score1: score1, score2: score2, currentTurn: turn, board: board })
        this.getMove()
      }
    }).catch(err => console.error(err))
  }

  componentDidMount() {

    switch(this.state.turn) {
      case '1':  // the one who creates game or is the host
        this.waitForOpponent()
        break;
      case '2':  // the one who joins game
        this.waitForSignalFromHost()
        break;
      case '3': // the one who spectates the game
        /* code for spectator */
        this.getMove()
        break;

      default: break;
    }
  }

  getResult() {

    if (!this.state.winByQuit) {
      if (this.state.winner !== '') {
        console.log('[WINNER] ' + this.state.winner)
        return ( <>Winner: {this.state.winner} </>)
      } else {
        return ( <>DRAW</>)
      }
    } else {
      return (<>{this.state.loser} quit. Winner: {this.state.winner}</>)
    }
  }

  showPopupWhenOpponentQuit() {
    if (this.state.turn === '1') {
      return (
        <h3>The oppenent quitted. Wait for another one...</h3>
      )
    } else if (this.state.turn === '2') {
      return (
        <h3>The host quitted. You are the host now. Wait for another one to start the game</h3>
      )
    }
  }

  showTurn() {
    if (this.state.currentTurn === '1') {
      return (
        <>Black</>
      )
    } else if (this.state.currentTurn === '2') {
      return (
        <>White</>
      )
    }
  }

  // confirmQuitGame() {
  //   this.setState({ showNotification })
  // }
  render() {
    return (
      <>
        <div className='room'>

          <Popup trigger={this.state.showNotification}>
            {this.showPopupWhenOpponentQuit()}
            <button className="start-btn" onClick={() => this.setState({ showNotification: false })}>OK</button>
          </Popup>
          <Popup trigger={this.state.showStart}>
            <h3>Start Game?</h3>
            <button className='start-btn' onClick={() => this.startGame()}>Start game</button>
          </Popup>
          <Popup trigger={this.state.showResult}>
            <h3>{this.getResult()}</h3>
            <button className='play-again' onClick={ () => this.playAgain() }>Play again</button>
            <button className='leave-room' onClick={() => this.quitGame() }>Leave room</button>
          </Popup>
          <Popup trigger={this.state.showResultForSpec}>
            <h3>{this.getResult()}</h3>
          </Popup>
          <header>
            <button className='quit' onClick={() => this.quitGame()}>Quit</button>
            <center>Turn: {this.showTurn()}</center>
          </header>
         
          <div className='game'>
            <div className="player1">
              <div className="black-logo"></div>
              <div className="player-info">
                <div className="score">{this.state.score1}</div>
                <div className="name">{this.state.host}</div>
              </div>
            </div>

            <div className="board-panel">
              <div className="board">
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
            
            <div className="player2">
              <div className="white-logo"></div>
              <div className="player-info">
                <div className="score">{this.state.score2}</div>
                <div className="name">{this.state.player2}</div>
              </div></div>
          </div>
        </div>
      </>
    )
  }
}

export default Game