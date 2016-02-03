/* pressure-bricklet
 * Copyright (C) 2016 Matthias Bolte <matthias@tinkerforge.com>
 *
 * pressure.h: Implementation of Pressure Bricklet messages
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef PRESSURE_H
#define PRESSURE_H

#include <stdint.h>
#include "bricklib/com/com.h"
#include "bricklib/com/com_common.h"

#define FID_GET_PRESSURE 1
#define FID_GET_ANALOG_VALUE 2
#define FID_SET_PRESSURE_CALLBACK_PERIOD 3
#define FID_GET_PRESSURE_CALLBACK_PERIOD 4
#define FID_SET_ANALOG_VALUE_CALLBACK_PERIOD 5
#define FID_GET_ANALOG_VALUE_CALLBACK_PERIOD 6
#define FID_SET_PRESSURE_CALLBACK_THRESHOLD 7
#define FID_GET_PRESSURE_CALLBACK_THRESHOLD 8
#define FID_SET_ANALOG_VALUE_CALLBACK_THRESHOLD 9
#define FID_GET_ANALOG_VALUE_CALLBACK_THRESHOLD 10
#define FID_SET_DEBOUNCE_PERIOD 11
#define FID_GET_DEBOUNCE_PERIOD 12
#define FID_SET_SENSOR_TYPE 13
#define FID_GET_SENSOR_TYPE 14
#define FID_SET_MOVING_AVERAGE 15
#define FID_GET_MOVING_AVERAGE 16
#define FID_PRESSURE 17
#define FID_ANALOG_VALUE 18
#define FID_PRESSURE_REACHED 19
#define FID_ANALOG_VALUE_REACHED 20

#define FID_LAST 16

#define SENSOR_TYPE_MPX5500 0
#define SENSOR_TYPE_MPXV5004 1
#define SENSOR_TYPE_MPX4115A 2

typedef struct {
	MessageHeader header;
	uint8_t sensor;
} __attribute__((__packed__)) SetSensorType;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetSensorType;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
} __attribute__((__packed__)) GetSensorTypeReturn;

typedef struct {
	MessageHeader header;
	uint8_t length;
} __attribute__((__packed__)) SetMovingAverage;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) GetMovingAverage;

typedef struct {
	MessageHeader header;
	uint8_t length;
} __attribute__((__packed__)) GetMovingAverageReturn;

int32_t analog_value_from_mc(const int32_t value);
int32_t pressure_from_analog_value(const int32_t value);
void set_sensor_type(const ComType com, const SetSensorType *data);
void get_sensor_type(const ComType com, const GetSensorType *data);
void set_moving_average(const ComType com, const SetMovingAverage *data);
void get_moving_average(const ComType com, const GetMovingAverage *data);
void reinitialize_moving_average(void);

void invocation(const ComType com, const uint8_t *data);
void constructor(void);
void destructor(void);
void tick(const uint8_t tick_type);

#endif
