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

# Register pressure callback (parameter has unit Pa)
p.register_callback(BrickletPressure::CALLBACK_PRESSURE) do |pressure|
  puts "Pressure: #{pressure/1000.0} kPa"
end

# Set period for pressure callback to 1s (1000ms)
# Note: The pressure callback is only called every second
#       if the pressure has changed since the last call!
p.set_pressure_callback_period 1000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
