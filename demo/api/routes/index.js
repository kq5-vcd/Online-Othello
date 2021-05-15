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



/* POST */
router.post('/username', (req, res) => {

  const recv = { username: req.body.username, message: req.body.message}
  let tmp = ''

  console.log("Recv: " + recv.username)

  client.write(recv.message + ' ' +  recv.username)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    console.log(res.headersSent)
    res.json({response: tmp})
  })
})

router.post('/loadRoom', (req, res) => {
  const recv = { message: req.body.message }
  let tmp = ''

  console.log('Recv: ' + recv.message)

  client.write(recv.message)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
  
    res.json({response: tmp})
  })
})

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

router.post('/join', (req, res) => {
  const recv = { message: req.body.message, roomID: req.body.roomID, player2: req.body.player2 }
  let tmp = ''

  console.log('Recv: ' + recv.message + ' - ' + recv.roomID + ' - ' + recv.player2)

  client.write(recv.message + ' ' + recv.roomID + ' ' + recv.player2)

  client.once('data', data => {
    console.log('Received from server: ' + data)
    tmp = data.toString()
    res.json({response: tmp})
  })
})


module.exports = router;
