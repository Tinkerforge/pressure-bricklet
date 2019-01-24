package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/pressure_bricklet"
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

	// Get threshold receivers with a debounce time of 10 seconds (10000ms).
	p.SetDebouncePeriod(10000)

	p.RegisterPressureReachedCallback(func(pressure int32) {
		fmt.Printf("Pressure: %f kPa\n", float64(pressure)/1000.0)
	})

	// Configure threshold for pressure "greater than 10 kPa".
	p.SetPressureCallbackThreshold('>', 10*1000, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
