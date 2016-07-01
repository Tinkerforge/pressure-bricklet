<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletPressure.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletPressure;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Pressure Bricklet

$ipcon = new IPConnection(); // Create IP connection
$p = new BrickletPressure(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current pressure (unit is Pa)
$pressure = $p->getPressure();
echo "Pressure: " . $pressure/1000.0 . " kPa\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
