#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_pressure.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change XYZ to the UID of your Pressure Bricklet

// Callback function for pressure callback (parameter has unit Pa)
void cb_pressure(int32_t pressure, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Pressure: %f kPa\n", pressure/1000.0);
}

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	Pressure p;
	pressure_create(&p, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Register pressure callback to function cb_pressure
	pressure_register_callback(&p,
	                           PRESSURE_CALLBACK_PRESSURE,
	                           (void *)cb_pressure,
	                           NULL);

	// Set period for pressure callback to 1s (1000ms)
	// Note: The pressure callback is only called every second
	//       if the pressure has changed since the last call!
	pressure_set_pressure_callback_period(&p, 1000);

	printf("Press key to exit\n");
	getchar();
	pressure_destroy(&p);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
