#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Pressure Bricklet

# Get current pressure
tinkerforge call pressure-bricklet $uid get-pressure
