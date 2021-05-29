var express = require('express');
var router = express.Router();
const client = require('../../network/client')

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

router.get('/testAPI', (req, res) => {
  res.send('API is working properly')
})

router.get('/ingame', (req, res) => {
  let tmp = ''

  client.once('data', data => {
    console.log("Received from server: " + data)
    tmp = data.toString()
    res.send(tmp)
  })
})

router.get('/getMove', (req, res) => {
  let tmp = ''

  client.once('data', data => {
    console.log("Received from server: " + data)
    tmp = data.toString()
    res.send(tmp)
  })
})


/* POST */

// Message 0
router.post('/username', (req, res) => {

  const recv = { username: req.body.username, message: req.body.message}
  let tmp = ''

  console.log("Recv: " + recv.username)

  client.write(recv.message + ' ' +  recv.username)

  console.log('Checked')

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({status: tmp})
  })
})

// Message 1
router.post('/loadRoom', (req, res) => {
  const recv = { message: req.body.message }
  let tmp = ''

  console.log('Recv: ' + recv.message)

  client.write(recv.message)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
  
    res.json({rooms: tmp})
  })
})

// Create new room
router.post('/create', (req, res) => {
  const recv = { message: req.body.message, username: req.body.username}
  let tmp = ''

  console.log('Recv: ' + recv.message + ' ' + recv.username)

  client.write(recv.message + ' ' + recv.username)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
})

// Message 3 - join room
router.post('/join', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID, player2: req.body.player2, player1: req.body.player1 }
  let tmp = ''

  console.log('Recv: ' + recv.message + ' - ' + recv.roomID + ' - ' + recv.player2)

  client.write(recv.message + ' ' + recv.player2 + ' ' + recv.roomID + ' ' + recv.player1)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
})

// Message 4 - start game
router.post('/start', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID}
  let tmp = ''
  console.log('here')

  client.write(recv.message + ' ' + recv.roomID)
  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })

})

// Message 5 - Move
router.post('/move', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID, move_row: req.body.move_row, move_col: req.body.move_col }
  let tmp = ''

  console.log('Recv: ' + recv.message + ' - ' + recv.roomID + ' - ' + recv.move_row + ' - ' + recv.move_col)

  client.write(recv.message + ' ' + recv.roomID + ' ' + recv.move_row + ' ' + recv.move_col)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
})

router.post('/quit', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID, state: req.body.state }

  let tmp = ''
  console.log('Recv: ' + recv.message + ' - ' + recv.roomID + ' - ' + recv.state)
  client.write(recv.message + ' ' + recv.roomID + ' ' + recv.state)

  client.once('data', data => {
    
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
})

// Message 7 - Play again
router.post('/playAgain', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID, turn: req.body.turn }

  let tmp = ''
  client.write(recv.message + ' ' + recv.roomID + ' ' +  recv.turn)

  client.once('data', data => {
    
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
  
})

module.exports = router;
