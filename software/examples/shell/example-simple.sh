#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get current pressure (unit is Pa)
tinkerforge call pressure-bricklet $uid get-pressure
