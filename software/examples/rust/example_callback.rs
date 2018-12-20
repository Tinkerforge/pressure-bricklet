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

    let pressure_receiver = p.get_pressure_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `p` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for pressure in pressure_receiver {
            println!("Pressure: {} kPa", pressure as f32 / 1000.0);
        }
    });

    // Set period for pressure receiver to 1s (1000ms).
    // Note: The pressure callback is only called every second
    //       if the pressure has changed since the last call!
    p.set_pressure_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
