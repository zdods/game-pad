// ********************
// SERVER SETUP
// ********************

const express = require("express")
const app = express()
const http = require("http").Server(app)
const bodyParser = require("body-parser")

const io = require("socket.io").listen(http)

const SerialPort = require("serialport")
const parsers = SerialPort.parsers

// USE bodyParser() -> get data from POST req
app.use(
  bodyParser.urlencoded({
    extended: true
  })
)
app.use(bodyParser.json())

// USE HELMET
app.use(require("helmet")())

// SERVE PUBLIC DIRECTORY
app.use(express.static(__dirname + "/public"))

// ********************
// ROUTES FOR OUR API
// ********************
const router = express.Router()

// MIDDLEWARE - auths 'n' stuff
router.use((req, res, next) => {
  next()
})

// REGISTER OUR ROUTES - prefix
app.use("/api", router)

// ********************
// START THE SERVER
// ********************

const parser = new parsers.Readline({
  delimiter: "\r\n"
})

var port = new SerialPort("/dev/cu.usbmodem14101", {
  baudRate: 9600,
  dataBits: 8,
  parity: "none",
  stopBits: 1,
  flowControl: false
})

port.pipe(parser)

io.on("connection", function(socket) {
  console.log("browser connected!")

  socket.on("bind", data => {
    port.write(JSON.stringify({ data }))
  })
})

parser.on("data", function(data) {
  console.log(data)
})

// PORT
const webPort = process.env.PORT || 1313 // set our port

http.listen(webPort)
console.log(`[PORT ${webPort}]: Is anyone there?`)
