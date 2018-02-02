function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletPressure;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Pressure Bricklet

    ipcon = IPConnection(); % Create IP connection
    p = handle(BrickletPressure(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current pressure
    pressure = p.getPressure();
    fprintf('Pressure: %g kPa\n', pressure/1000.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
