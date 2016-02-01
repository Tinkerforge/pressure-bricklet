var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var p = new Tinkerforge.BrickletPressure(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        p.setDebouncePeriod(10000);

        // Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
        p.setPressureCallbackThreshold('>', 10*1000, 0);
    }
);

// Register pressure reached callback
p.on(Tinkerforge.BrickletPressure.CALLBACK_PRESSURE_REACHED,
    // Callback function for pressure reached callback (parameter has unit Pa)
    function (pressure) {
        console.log('Pressure: ' + pressure/1000.0 + ' kPa');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
