package main

import (
	"fmt"
	"tinkerforge/ipconnection"
	"tinkerforge/pressure_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Pressure Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	p, _ := pressure_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current pressure.
	pressure, _ := p.GetPressure()
	fmt.Printf("Pressure: %f kPa\n", float64(pressure)/1000.0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
