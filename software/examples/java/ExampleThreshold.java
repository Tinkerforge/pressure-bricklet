import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletPressure;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Pressure Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletPressure p = new BrickletPressure(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		p.setDebouncePeriod(10000);

		// Add pressure reached listener
		p.addPressureReachedListener(new BrickletPressure.PressureReachedListener() {
			public void pressureReached(int pressure) {
				System.out.println("Pressure: " + pressure/1000.0 + " kPa");
			}
		});

		// Configure threshold for pressure "greater than 10 kPa"
		p.setPressureCallbackThreshold('>', 10*1000, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
