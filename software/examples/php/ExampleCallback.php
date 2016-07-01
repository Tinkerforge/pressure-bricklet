<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPressure.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPressure;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Pressure Bricklet

// Callback function for pressure callback (parameter has unit Pa)
function cb_pressure($pressure)
{
    echo "Pressure: " . $pressure/1000.0 . " kPa\n";
}

$ipcon = new IPConnection(); // Create IP connection
$p = new BrickletPressure(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register pressure callback to function cb_pressure
$p->registerCallback(BrickletPressure::CALLBACK_PRESSURE, 'cb_pressure');

// Set period for pressure callback to 1s (1000ms)
// Note: The pressure callback is only called every second
//       if the pressure has changed since the last call!
$p->setPressureCallbackPeriod(1000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
