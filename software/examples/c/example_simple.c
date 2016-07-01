#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_pressure.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change XYZ to the UID of your Pressure Bricklet

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

	// Get current pressure (unit is Pa)
	int32_t pressure;
	if(pressure_get_pressure(&p, &pressure) < 0) {
		fprintf(stderr, "Could not get pressure, probably timeout\n");
		return 1;
	}

	printf("Pressure: %f kPa\n", pressure/1000.0);

	printf("Press key to exit\n");
	getchar();
	pressure_destroy(&p);
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
