#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Pressure Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_pressure import BrickletPressure

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    p = BrickletPressure(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current pressure (unit is Pa)
    pressure = p.get_pressure()
    print("Pressure: " + str(pressure/1000.0) + " kPa")

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
