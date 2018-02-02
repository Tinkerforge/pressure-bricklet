#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Pressure Bricklet

# Handle incoming pressure callbacks
tinkerforge dispatch pressure-bricklet $uid pressure &

# Set period for pressure callback to 1s (1000ms)
# Note: The pressure callback is only called every second
#       if the pressure has changed since the last call!
tinkerforge call pressure-bricklet $uid set-pressure-callback-period 1000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
