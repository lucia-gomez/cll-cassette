// variable to hold an instance of the p5.webserial library:
const serial = new p5.WebSerial();

// HTML button object:
let portButton;

function serialSetup() {
  // check to see if serial is available:
  if (!navigator.serial) {
    alert("WebSerial is not supported in this browser. Try Chrome or MS Edge.");
  }
  // if serial is available, add connect/disconnect listeners:
  navigator.serial.addEventListener("connect", portConnect);
  navigator.serial.addEventListener("disconnect", portDisconnect);
  // check for any ports that are available:
  serial.getPorts();
  // if there's no port chosen, choose one:
  serial.on("noport", makePortButton);
  // open whatever port is available:
  serial.on("portavailable", openPort);
  // handle serial errors:
  serial.on("requesterror", portError);
  // handle any incoming serial data:
  serial.on("data", serialEvent);
  serial.on("close", makePortButton);
}

// if there's no port selected,
// make a port select button appear:
function makePortButton() {
  // create and position a port chooser button:
  portButton = createButton("choose port");
  portButton.position(10, 650);
  // give the port button a mousepressed handler:
  portButton.mousePressed(choosePort);
}

// make the port selector window appear:
function choosePort() {
  serial.requestPort();
}

// open the selected port, and make the port
// button invisible:
function openPort() {
  // wait for the serial.open promise to return,
  // then call the initiateSerial function
  serial.open().then(initiateSerial);

  // once the port opens, let the user know:
  function initiateSerial() {
    console.log("port open");
    // serial.write("x");
  }
  // hide the port button once a port is chosen:
  // if (portButton) portButton.hide();
}

// read any incoming data:
// NOT WORKING
function serialEvent() {
  const inString = serial.readLine();

  if (inString) {
    const currentString = inString.trim(); // Trim whitespace
    console.log("Received serial data:", currentString); // Log the received string

    // Process the received string
    if (currentString[0] === "p" || currentString[0] === "P") {
      // Example action for "P"
      console.log("Action for P");
      // cassette.legRight.addPixels(); // Uncomment when cassette is defined
    } else if (currentString[0] === "q" || currentString[0] === "Q") {
      // Example action for "Q"
      console.log("Action for Q");
      // cassette.legLeft.addPixels(); // Uncomment when cassette is defined
    }
  }
}

// pop up an alert if there's a port error:
function portError(err) {
  alert("Serial port error: " + err);
}

// try to connect if a new serial port
// gets added (i.e. plugged in via USB):
function portConnect() {
  console.log("port connected");
  serial.getPorts();
}

// if a port is disconnected:
function portDisconnect() {
  serial.close();
  console.log("port disconnected");
}
