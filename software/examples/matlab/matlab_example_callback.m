function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPressure;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    p = handle(BrickletPressure(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register pressure callback to function cb_pressure
    set(p, 'PressureCallback', @(h, e) cb_pressure(e));

    % Set period for pressure callback to 1s (1000ms)
    % Note: The pressure callback is only called every second
    %       if the pressure has changed since the last call!
    p.setPressureCallbackPeriod(1000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for pressure callback (parameter has unit Pa)
function cb_pressure(e)
    fprintf('Pressure: %g kPa\n', e.pressure/1000.0);
end
