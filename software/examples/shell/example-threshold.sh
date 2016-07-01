#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Pressure Bricklet

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call pressure-bricklet $uid set-debounce-period 10000

# Handle incoming pressure reached callbacks (parameter has unit Pa)
tinkerforge dispatch pressure-bricklet $uid pressure-reached &

# Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
tinkerforge call pressure-bricklet $uid set-pressure-callback-threshold greater 10000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
