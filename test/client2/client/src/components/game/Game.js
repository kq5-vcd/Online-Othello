import React from 'react'
import axios from 'axios'
import ReactDOM from 'react-dom'
import Popup from './Popup'
import Multi from '../multi/Multi'

class Game extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      roomID: this.props.roomID,
      board: this.props.board,
      turn: this.props.turn,
      player1: this.props.player1,
      player2: this.props.player2,
      score1: '2',
      score2: '2',
      showStart: false,
      showResult: false,
      isPlaying: '0'
    }
  }

  // start game
  startGame() {
    console.log('Start game')
    
    // Send: ("4" <room_id>)
    // Receive: (<board> <turn> <player1> <player2> <score1> <score2>)
    const req = { message: '4', roomID: this.props.roomID }

    axios.post('http://localhost:9001/start', req)
      .then(res => {
        const tmp = res.data.response.split(' ')

        const board = tmp.slice(0,64)
        const turn = tmp[64]
        const player1 = tmp[65]
        const player2 = tmp[66]
        const score1 = tmp[67]
        const score2 = tmp[68]

        this.setState({ board: board, player1: player1, player2: player2, score1: score1, score2: score2, showStart: false, isPlaying: '1' })
      }).catch(err => console.error(err))
  }

  // get the opponent's move
  getMove() {
    // Receive: (<board> <turn> <player1> <player2> <score1> <score2>)
    fetch('http://localhost:9001/getMove')
      .then(res => res.text())
      .then(res => {

        const tmp = res.split(' ')
        const board = tmp.slice(0,64)
        const turn = tmp[64]
        const player1 = tmp[65]
        const player2 = tmp[66]
        const score1 = tmp[67]
        const score2 = tmp[68]
        this.setState({ board: board, player1: player1, player2: player2, score1: score1 , score2: score2})

        if (turn !== this.state.turn && turn !== '-1') { // If game hasn't finished
          this.getMove()
        } else if (turn === '-1') {   // if game finished
          this.setState({ showResult: true, isPlaying: '0'})
        }
      }).catch(err => console.error(err))
  }

  quitGame() {
    // SEnd: ("6" <room_id> <isPlaying>)
    // Then, call loadroom()

    console.log('Is game playing? - ' + this.state.isPlaying)
    const req = { message: '6', roomID: this.props.roomID, isPlaying: this.state.isPlaying }
    console.log('Quit game: ')
    console.log(req)

    axios.post('http://localhost:9001/quit', req)
      .then(res => {
        if (res.data.response === '0') {
          ReactDOM.render(<Multi username={this.props.username} />, document.getElementById('root'))
        }
      }).catch(err => console.error(err))
  }

  playAgain() {
    // SEND: ("7" <room_id> <turn>)
    // RECEIVE: 
    const req = { message: '7', roomID: this.props.roomID }

    axios.post('http://localhost:9001/playAgain', req)
      .then(res => {
        const tmp = res.data.response.split(' ')

        const board = tmp.slice(0,64)
        const turn = tmp[64]
        this.setState({ board: board, showResult: false})
        console.log('Play again: ' + turn)

        switch(turn) {
          case '0': // clicked 'Play again' first and wait for the other
            fetch('http://localhost:9001/ingame')
              .then(res => res.text())
              .then(res => {

                // if the other quit game, RECEIVE: (<board> 0)
                const turn = res.split(' ')[64]
                console.log('Turn: ' + turn)
                
                
                switch(turn) {
                  case '0':
                    this.setState({ turn: '1', player1: this.props.username })
                    fetch('http://localhost:9001/ingame') // wait for other to join
                      .then(res => res.text())
                      .then(res => {
                        this.setState({ player2: res, showStart: true})
                      }).catch(err => console.error(err))
                    break;
                  case '1': 
                    this.setState({ showStart: true})
                    break;
                  case '2':
                    fetch('http://localhost:9001/ingame')
                      .then(res => res.text())
                      .then(res => {
                        this.setState({ isPlaying: '1'})
                        this.getMove()
                      }).catch(err => console.error(err))
                    break;
                  default: break;
                }

              })
            break;
          case '1':
            console.log('You clicked after and your turn is 1')
            this.setState({ showStart: true})
            break;
            // const player1 = res[65]
            // this.setState({ player1: player1, turn: turn})
            // // wait for other join
            // fetch('http://localhost:9001/ingame')
            //   .then(res => res.text())
            //   .then(res => {
            //     this.setState({ player2: res, showStart: true})
            //   }).catch(err => console.error(err))
          
            case '2':
              console.log('You clicked after and your turn is 2')
              fetch('http://localhost:9001/ingame')
                .then(res => res.text())
                .then(res => {
                  this.setState({ isPlaying: true})
                  this.getMove()
                }).catch(err => console.error(err))
              break;

            case '3':  // the other quit
              console.log('The other quit')
              if (this.state.turn === '1') {
                fetch('http://localhost:9001/ingame')
                  .then(res => res.text())
                  .then(res => {
                    this.setState({ player2: res, showStart: true})
                  }).catch(err => console.error(err))
              } else if (this.state.turn === '2') {
                this.setState({ turn: '1', player1: this.props.username, player2: ''})
                fetch('http://localhost:9001/ingame')
                  .then(res => res.text())
                  .then(res => {
                    this.setState({ player2: res, showStart: true})
                  }).catch(err => console.error(err))
              }
              break;
          
          default: break;

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
        .post('http://localhost:9001/move', req)
        .then(res => {
          const board = res.data.response.split(' ').slice(0,64)
          const score1 = res.data.response.split(' ')[67]
          const score2 = res.data.response.split(' ')[68]
          const turn = res.data.response.split(' ')[64]
          this.setState({ board: board, score1: score1, score2: score2 })
          
          if (turn !== this.state.turn && turn !== '-1')  { // not your turn
            this.getMove()
          } else if (turn === '-1') { // game ends
            this.setState({ showResult: true, isPlaying: '0'})
          }
        }).catch(err => console.error(err))
  }

  // 2 ways: 
  // - create new game: turn = 0 -> wait for /ingame
  // - join a game: turn = 2 -> wait for the opponent's move
  componentDidMount() {

    console.log('Is playing? - ' + this.state.isPlaying )

    if (this.state.turn === '1') { // the one create the game or host
      
      // Receive: (<player2>)
      fetch('http://localhost:9001/ingame')  // wait for an opponent and show the start box
        .then(res => res.text())
        .then(res => {
          this.setState({ player2: res, showStart: true})
        }).catch(err => console.error(err))

    } else if (this.state.turn === '2') {  // the one who joins the game or not the host

      // Receive: 
      fetch('http://localhost:9001/ingame')
        .then(res => res.text())
        .then(res => {
          this.setState({ isPlaying: '1'})
          this.getMove()
        }).catch(err => console.error(err))
    }
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
          <Popup trigger={this.state.showStart}>
            <h3>Start Game?</h3>
            <button className='start-btn' onClick={() => this.startGame()}>Start game</button>
          </Popup>
          <Popup trigger={this.state.showResult}>
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