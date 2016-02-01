#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_pressure.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback function for pressure reached callback (parameter has unit Pa)
void cb_pressure_reached(int32_t pressure, void *user_data) {
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

	// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
	pressure_set_debounce_period(&p, 10000);

	// Register pressure reached callback to function cb_pressure_reached
	pressure_register_callback(&p,
	                           PRESSURE_CALLBACK_PRESSURE_REACHED,
	                           (void *)cb_pressure_reached,
	                           NULL);

	// Configure threshold for pressure "greater than 10 kPa" (unit is Pa)
	pressure_set_pressure_callback_threshold(&p, '>', 10*1000, 0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
