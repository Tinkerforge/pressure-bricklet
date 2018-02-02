#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_pressure'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Pressure Bricklet

ipcon = IPConnection.new # Create IP connection
p = BrickletPressure.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current pressure
pressure = p.get_pressure
puts "Pressure: #{pressure/1000.0} kPa"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
