#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Pressure Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_pressure import BrickletPressure

# Callback function for pressure callback (parameter has unit Pa)
def cb_pressure(pressure):
    print("Pressure: " + str(pressure/1000.0) + " kPa")

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    p = BrickletPressure(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Register pressure callback to function cb_pressure
    p.register_callback(p.CALLBACK_PRESSURE, cb_pressure)

    # Set period for pressure callback to 1s (1000ms)
    # Note: The pressure callback is only called every second
    #       if the pressure has changed since the last call!
    p.set_pressure_callback_period(1000)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
