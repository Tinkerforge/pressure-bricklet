#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_pressure import BrickletPressure

# Callback function for pressure reached callback (parameter has unit Pa)
def cb_pressure_reached(pressure):
    print("Pressure: " + str(pressure/1000.0) + " kPa")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    p = BrickletPressure(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    p.set_debounce_period(10000)

    # Register pressure reached callback to function cb_pressure_reached
    p.register_callback(p.CALLBACK_PRESSURE_REACHED, cb_pressure_reached)

    # Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
    p.set_pressure_callback_threshold(">", 10*1000, 0)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
