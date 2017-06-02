function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Pressure Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    p = javaObject("com.tinkerforge.BrickletPressure", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current pressure (unit is Pa)
    pressure = p.getPressure();
    fprintf("Pressure: %g kPa\n", pressure/1000.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
