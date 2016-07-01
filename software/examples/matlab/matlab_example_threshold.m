function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPressure;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Pressure Bricklet

    ipcon = IPConnection(); % Create IP connection
    p = handle(BrickletPressure(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    p.setDebouncePeriod(10000);

    % Register pressure reached callback to function cb_pressure_reached
    set(p, 'PressureReachedCallback', @(h, e) cb_pressure_reached(e));

    % Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
    p.setPressureCallbackThreshold('>', 10*1000, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for pressure reached callback (parameter has unit Pa)
function cb_pressure_reached(e)
    fprintf('Pressure: %g kPa\n', e.pressure/1000.0);
end
