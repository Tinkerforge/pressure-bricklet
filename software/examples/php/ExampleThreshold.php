<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPressure.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPressure;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Pressure Bricklet

// Callback function for pressure reached callback
function cb_pressureReached($pressure)
{
    echo "Pressure: " . $pressure/1000.0 . " kPa\n";
}

$ipcon = new IPConnection(); // Create IP connection
$p = new BrickletPressure(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$p->setDebouncePeriod(10000);

// Register pressure reached callback to function cb_pressureReached
$p->registerCallback(BrickletPressure::CALLBACK_PRESSURE_REACHED, 'cb_pressureReached');

// Configure threshold for pressure "greater than 10 kPa"
$p->setPressureCallbackThreshold('>', 10*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
