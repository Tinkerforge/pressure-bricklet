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
        // Set period for pressure callback to 1s (1000ms)
        // Note: The pressure callback is only called every second
        //       if the pressure has changed since the last call!
        p.setPressureCallbackPeriod(1000);
    }
);

// Register pressure callback
p.on(Tinkerforge.BrickletPressure.CALLBACK_PRESSURE,
    // Callback function for pressure callback (parameter has unit Pa)
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
