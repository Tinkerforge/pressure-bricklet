use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, pressure_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Pressure Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let p = PressureBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get threshold receivers with a debounce time of 10 seconds (10000ms).
    p.set_debounce_period(10000);

    // Create receiver for pressure reached events.
    let pressure_reached_receiver = p.get_pressure_reached_receiver();

    // Spawn thread to handle received events. This thread ends when the `p` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for pressure_reached in pressure_reached_receiver {
            println!("Pressure: {} kPa", pressure_reached as f32 / 1000.0);
        }
    });

    // Configure threshold for pressure "greater than 10 kPa".
    p.set_pressure_callback_threshold('>', 10 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
