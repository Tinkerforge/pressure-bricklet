import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletPressure;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPressure p = new BrickletPressure(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current pressure (unit is Pa)
		int pressure = p.getPressure(); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Pressure: " + pressure/1000.0 + " kPa");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
