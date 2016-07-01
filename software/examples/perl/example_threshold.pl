#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletPressure;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Pressure Bricklet

# Callback subroutine for pressure reached callback (parameter has unit Pa)
sub cb_pressure_reached
{
    my ($pressure) = @_;

    print "Pressure: " . $pressure/1000.0 . " kPa\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $p = Tinkerforge::BrickletPressure->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$p->set_debounce_period(10000);

# Register pressure reached callback to subroutine cb_pressure_reached
$p->register_callback($p->CALLBACK_PRESSURE_REACHED, 'cb_pressure_reached');

# Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
$p->set_pressure_callback_threshold('>', 10*1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
