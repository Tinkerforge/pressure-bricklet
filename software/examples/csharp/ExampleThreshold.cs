using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Pressure Bricklet

	// Callback function for pressure reached callback (parameter has unit Pa)
	static void PressureReachedCB(BrickletPressure sender, int pressure)
	{
		Console.WriteLine("Pressure: " + pressure/1000.0 + " kPa");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPressure p = new BrickletPressure(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		p.SetDebouncePeriod(10000);

		// Register pressure reached callback to function PressureReachedCB
		p.PressureReached += PressureReachedCB;

		// Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
		p.SetPressureCallbackThreshold('>', 10*1000, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
