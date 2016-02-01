import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletPressure;

public class ExampleCallback {
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

		// Add pressure listener (parameter has unit Pa)
		p.addPressureListener(new BrickletPressure.PressureListener() {
			public void pressure(int pressure) {
				System.out.println("Pressure: " + pressure/1000.0 + " kPa");
			}
		});

		// Set period for pressure callback to 1s (1000ms)
		// Note: The pressure callback is only called every second
		//       if the pressure has changed since the last call!
		p.setPressureCallbackPeriod(1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
