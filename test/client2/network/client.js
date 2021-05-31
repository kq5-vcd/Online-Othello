const net = require('net')

const client = net.connect(8910, () => {
    console.log('connected')
})

module.exports = client