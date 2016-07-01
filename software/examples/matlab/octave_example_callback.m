function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Pressure Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    p = java_new("com.tinkerforge.BrickletPressure", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register pressure callback to function cb_pressure
    p.addPressureCallback(@cb_pressure);

    % Set period for pressure callback to 1s (1000ms)
    % Note: The pressure callback is only called every second
    %       if the pressure has changed since the last call!
    p.setPressureCallbackPeriod(1000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for pressure callback (parameter has unit Pa)
function cb_pressure(e)
    fprintf("Pressure: %g kPa\n", e.pressure/1000.0);
end
