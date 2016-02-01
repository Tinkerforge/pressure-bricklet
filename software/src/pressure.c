/* pressure-bricklet
 * Copyright (C) 2016 Matthias Bolte <matthias@tinkerforge.com>
 *
 * pressure.c: Implementation of Pressure Bricklet messages
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

#include "pressure.h"

#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/drivers/adc/adc.h"
#include "bricklib/utility/util_definitions.h"
#include "brickletlib/bricklet_entry.h"
#include "brickletlib/bricklet_simple.h"
#include "config.h"

#define PRESSURE_AVERAGE 50

#define SIMPLE_UNIT_PRESSURE 0
#define SIMPLE_UNIT_ANALOG_VALUE 1

const SimpleMessageProperty smp[] = {
	{SIMPLE_UNIT_PRESSURE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_PRESSURE
	{SIMPLE_UNIT_ANALOG_VALUE, SIMPLE_TRANSFER_VALUE, SIMPLE_DIRECTION_GET}, // TYPE_GET_ANALOG_VALUE
	{SIMPLE_UNIT_PRESSURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_PRESSURE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_PRESSURE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ANALOG_VALUE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ANALOG_VALUE_CALLBACK_PERIOD
	{SIMPLE_UNIT_ANALOG_VALUE, SIMPLE_TRANSFER_PERIOD, SIMPLE_DIRECTION_GET}, // TYPE_GET_ANALOG_VALUE_CALLBACK_PERIOD
	{SIMPLE_UNIT_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_PRESSURE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_PRESSURE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ANALOG_VALUE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_SET}, // TYPE_SET_ANALOG_VALUE_CALLBACK_THRESHOLD
	{SIMPLE_UNIT_ANALOG_VALUE, SIMPLE_TRANSFER_THRESHOLD, SIMPLE_DIRECTION_GET}, // TYPE_GET_ANALOG_VALUE_CALLBACK_THRESHOLD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_SET}, // TYPE_SET_DEBOUNCE_PERIOD
	{0, SIMPLE_TRANSFER_DEBOUNCE, SIMPLE_DIRECTION_GET}, // TYPE_GET_DEBOUNCE_PERIOD
};

const SimpleUnitProperty sup[] = {
	{pressure_from_analog_value, SIMPLE_SIGNEDNESS_INT, FID_PRESSURE, FID_PRESSURE_REACHED, SIMPLE_UNIT_ANALOG_VALUE}, // pressure
	{analog_value_from_mc, SIMPLE_SIGNEDNESS_UINT, FID_ANALOG_VALUE, FID_ANALOG_VALUE_REACHED, SIMPLE_UNIT_ANALOG_VALUE}, // analog value
};

const uint8_t smp_length = sizeof(smp);

void invocation(const ComType com, const uint8_t *data) {
	switch(((SimpleStandardMessage*)data)->header.fid) {
		case FID_SET_SENSOR_TYPE: {
			set_sensor_type(com, (SetSensorType*)data);
			return;
		}

		case FID_GET_SENSOR_TYPE: {
			get_sensor_type(com, (GetSensorType*)data);
			return;
		}

		case FID_SET_MOVING_AVERAGE: {
			set_moving_average(com, (SetMovingAverage*)data);
			return;
		}

		case FID_GET_MOVING_AVERAGE: {
			get_moving_average(com, (GetMovingAverage*)data);
			return;
		}

		default: {
			simple_invocation(com, data);
			break;
		}
	}

	if(((SimpleStandardMessage*)data)->header.fid > FID_LAST) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_NOT_SUPPORTED, com);
	}
}

void constructor(void) {
	_Static_assert(sizeof(BrickContext) <= BRICKLET_CONTEXT_MAX_SIZE, "BrickContext too big");

	BC->sensor = SENSOR_TYPE_MPX5500;

	PIN_AD.type = PIO_INPUT;
	PIN_AD.attribute = PIO_DEFAULT;
	BA->PIO_Configure(&PIN_AD, 1);

	// FIXME: prototype has 5V boost converter EN pin connected. remove this
	//        for production version which will be always on
	PIN_EN.type = PIO_OUTPUT_1;
	BA->PIO_Configure(&PIN_EN, 1);

	adc_channel_enable(BS->adc_channel);
	SLEEP_MS(2); // FIXME: different sensors might have different warm-up times

	BC->moving_average_upto = MAX_MOVING_AVERAGE;
	reinitialize_moving_average();

	simple_constructor();
}

void destructor(void) {
	simple_destructor();
	adc_channel_disable(BS->adc_channel);
}

void reinitialize_moving_average(void) {
	int32_t initial_value = BA->adc_channel_get_data(BS->adc_channel);
	for(uint8_t i = 0; i < BC->moving_average_upto; i++) {
		BC->moving_average[i] = initial_value;
	}
	BC->moving_average_tick = 0;
	BC->moving_average_sum = initial_value*BC->moving_average_upto;
}

int32_t analog_value_from_mc(const int32_t value) {
	uint16_t analog_data = BA->adc_channel_get_data(BS->adc_channel);
	BC->moving_average_sum = BC->moving_average_sum -
	                         BC->moving_average[BC->moving_average_tick] +
	                         analog_data;

	BC->moving_average[BC->moving_average_tick] = analog_data;
	BC->moving_average_tick = (BC->moving_average_tick + 1) % BC->moving_average_upto;

	return (BC->moving_average_sum + BC->moving_average_upto/2)/BC->moving_average_upto;
}

int32_t pressure_from_analog_value(const int32_t value) {
	// assumed values are operating voltage of 3.3V and 33/51 opamp multiplier
	int32_t voltage = (MAX_ADC_VALUE - value) * 3300 * 51 / (MAX_ADC_VALUE * 31);
	int32_t pressure = 0;

	if (BC->sensor == SENSOR_TYPE_MPX5500) {
		// MPX5500 (0-500000 Pa)
		// V  = 5.0 * (0.0018 * kPa + 0.04)
		// Pa = (2000 * mV - 400000) / 18
		pressure = (2000 * voltage - 400000) / 18;

		// We only have resolution of ~100 Pa, remove unreliable counts
		int8_t last_two_digits = pressure % 100;

		if (last_two_digits >= 0) {
			if (last_two_digits >= 50) {
				pressure += 100 - last_two_digits;
			} else {
				pressure -= last_two_digits;
			}
		} else {
			if (last_two_digits <= -50) {
				pressure -= 100 + last_two_digits;
			} else {
				pressure += -last_two_digits;
			}
		}
	} else if (BC->sensor == SENSOR_TYPE_MPXV5004) {
		// MPXV5004 (0-3920 Pa)
		// V  = 5.0 * (0.2 * kPa + 0.2)
		// Pa = mV - 1000
		pressure = voltage - 1000;
	}

	return pressure;
}

void set_sensor_type(const ComType com, const SetSensorType *data) {
	if (data->sensor > SENSOR_TYPE_MPXV5004) {
		BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
		return;
	}

	BC->sensor = data->sensor;

	BA->com_return_setter(com, data);
}

void get_sensor_type(const ComType com, const GetSensorType *data) {
	GetSensorTypeReturn gstr;

	gstr.header        = data->header;
	gstr.header.length = sizeof(gstr);
	gstr.sensor        = BC->sensor;

	BA->send_blocking_with_timeout(&gstr, sizeof(gstr), com);
}

void set_moving_average(const ComType com, const SetMovingAverage *data) {
	if(BC->moving_average_upto != data->length) {
		if(data->length < 1) {
			BC->moving_average_upto = 1;
		} else if(data->length > MAX_MOVING_AVERAGE) {
			BC->moving_average_upto = MAX_MOVING_AVERAGE;
		} else {
			BC->moving_average_upto = data->length;
		}

		reinitialize_moving_average();
	}

	BA->com_return_setter(com, data);
}

void get_moving_average(const ComType com, const GetMovingAverage *data) {
	GetMovingAverageReturn gmar;

	gmar.header        = data->header;
	gmar.header.length = sizeof(gmar);
	gmar.length        = BC->moving_average_upto;

	BA->send_blocking_with_timeout(&gmar, sizeof(gmar), com);
}

void tick(const uint8_t tick_type) {
	simple_tick(tick_type);
}
