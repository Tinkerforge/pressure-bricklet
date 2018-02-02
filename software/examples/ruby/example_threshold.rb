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

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
p.set_debounce_period 10000

# Register pressure reached callback
p.register_callback(BrickletPressure::CALLBACK_PRESSURE_REACHED) do |pressure|
  puts "Pressure: #{pressure/1000.0} kPa"
end

# Configure threshold for pressure "greater than 10 kPa"
p.set_pressure_callback_threshold '>', 10*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
