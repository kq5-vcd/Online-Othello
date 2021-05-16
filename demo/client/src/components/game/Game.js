import React from 'react'
import Board from './Board'
import axios from 'axios'

class Game extends React.Component {
  constructor(props) {
    super(props)
    this.state = {
      message: '',
      roomID: this.props.roomID
    }
  }

  getScore() {
    const board = this.props.board.split(' ').slice(0, 64)
    let p1 = 0
    let p2 = 0

    for (var i = 0; i < 64; i++) {
      if (board[i] == 1) p1++
      if (board[i] == 2) p2++
    }

    return (
      <>
      {p1} - {p2}
      </>
    )

  }

  winner() {
    if (this.props.board.split(' ')[64] === '-1') {
      if (this.props.board.split(' ')[67] < this.props.board.split(' ')[68]) {
        return (
          <>
            {this.props.board.split(' ')[66]} won!!!!
          </>
        )
      } else if (this.props.board.split(' ')[67] > this.props.board.split(' ')[68]) {
        return (
          <>
            {this.props.board.split(' ')[65]} won!!!!
          </>
        )
      } else {
        return (
          <>
            DRAW!!!
          </>
        )
      }
    }
  }

  render() {

    console.log("Render: " + this.props.board)
    return(
      <>
        <div>
          ROOM: {this.state.roomID}<br />
          {this.props.board.split(' ')[65]} : {this.getScore()} : {this.props.board.split(' ')[66]}
          <br/>{this.winner()}
        </div>
        <div className="game">
          <Board squares={this.props.board.split(' ').slice(0,64)} roomID={this.props.roomID} />
        </div>

        {/* {console.log(this.props.board.split(' '))} */}
      </>
    )
  }
}
// const Game = () => {

//     const [history, setHistory] = useState([Array(64).fill(null)])
//     const [stepNumber, setStepNumber] = useState(0)
//     const [xIsNext, setXisNext] = useState(true)
//     const winner = calculateWinner(history[stepNumber]);
//     const xO = xIsNext ? "X" : "O";

//     history[stepNumber][27] = 'O'
//     history[stepNumber][36] = 'O'
//     history[stepNumber][28] = 'X'
//     history[stepNumber][35] = 'X'

//     const handleClick = (i) => {
//         const historyPoint = history.slice(0, stepNumber + 1);
//         const current = historyPoint[stepNumber];
//         const squares = [...current];
    
//         // return if won or occupied
//         if (winner || squares[i]) return;
    
//         // select square
//         squares[i] = xO;
        
    
//         setHistory([...historyPoint, squares]);
//         setStepNumber(historyPoint.length);
//         setXisNext(!xIsNext);
    
       
//       };
    
//       const jumpTo = (step) => {
//         setStepNumber(step);
//         setXisNext(step % 2 === 0);
//       };
    
//       const renderMoves = () =>
//         history.map((_step, move) => {
//           const destination = move ? `Go to move #${move}` : "Go to Start";
//           return (
//             <li key={move}>
//               <button onClick={() => jumpTo(move)}>{destination}</button>
//             </li>
//           );
//         });
    
//       return (
//           <>
//             <div className="game">
//                 <button>Quit</button>
//             </div>

            
//             <div className="header">
//                 <h2>OTHELLO ONLINE </h2>
//                 {/* <Board squares={history[stepNumber]} onClick={handleClick} /> */}
//             </div>
//             {/* <div className="info-wrapper">
//                 <div>
//                     <h3>History</h3>
//                     {renderMoves()} 
          
//                 </div>
//             <h3>{winner ? "Winner: " + winner : "Next Player: " + xO}</h3>
//         </div> */}
        
//         </>
//       );


// }

export default Game