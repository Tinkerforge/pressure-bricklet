using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Pressure Bricklet

	// Callback function for pressure callback (parameter has unit Pa)
	static void PressureCB(BrickletPressure sender, int pressure)
	{
		Console.WriteLine("Pressure: " + pressure/1000.0 + " kPa");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPressure p = new BrickletPressure(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register pressure callback to function PressureCB
		p.Pressure += PressureCB;

		// Set period for pressure callback to 1s (1000ms)
		// Note: The pressure callback is only called every second
		//       if the pressure has changed since the last call!
		p.SetPressureCallbackPeriod(1000);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
