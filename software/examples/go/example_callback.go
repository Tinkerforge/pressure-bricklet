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

	p.RegisterPressureCallback(func(pressure int32) {
		fmt.Printf("Pressure: %f kPa\n", float64(pressure)/1000.0)
	})

	// Set period for pressure receiver to 1s (1000ms).
	// Note: The pressure callback is only called every second
	//       if the pressure has changed since the last call!
	p.SetPressureCallbackPeriod(1000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
