use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, pressure_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Pressure Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let pressure_bricklet = PressureBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get threshold listeners with a debounce time of 10 seconds (10000ms)
    pressure_bricklet.set_debounce_period(10000);

    //Create listener for pressure reached events.
    let pressure_reached_listener = pressure_bricklet.get_pressure_reached_receiver();
    // Spawn thread to handle received events. This thread ends when the pressure_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in pressure_reached_listener {
            println!("Pressure: {}{}", event as f32 / 1000.0, " kPa");
        }
    });

    // Configure threshold for pressure "greater than 10 kPa"
    pressure_bricklet.set_pressure_callback_threshold('>', 10 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
