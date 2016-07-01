#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletPressure;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Pressure Bricklet

# Callback subroutine for pressure callback (parameter has unit Pa)
sub cb_pressure
{
    my ($pressure) = @_;

    print "Pressure: " . $pressure/1000.0 . " kPa\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $p = Tinkerforge::BrickletPressure->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register pressure callback to subroutine cb_pressure
$p->register_callback($p->CALLBACK_PRESSURE, 'cb_pressure');

# Set period for pressure callback to 1s (1000ms)
# Note: The pressure callback is only called every second
#       if the pressure has changed since the last call!
$p->set_pressure_callback_period(1000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
