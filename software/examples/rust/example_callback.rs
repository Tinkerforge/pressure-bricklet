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

    //Create listener for pressure events.
    let pressure_listener = pressure_bricklet.get_pressure_receiver();
    // Spawn thread to handle received events. This thread ends when the pressure_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in pressure_listener {
            println!("Pressure: {}{}", event as f32 / 1000.0, " kPa");
        }
    });

    // Set period for pressure listener to 1s (1000ms)
    // Note: The pressure callback is only called every second
    //       if the pressure has changed since the last call!
    pressure_bricklet.set_pressure_callback_period(1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
