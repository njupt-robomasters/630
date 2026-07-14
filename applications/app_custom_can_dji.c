/*
	Copyright 2024 - 2026 NJUPT Potential

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "app.h"
#include "ch.h"
#include "hal.h"
#include "comm_can.h"
#include "mc_interface.h"
#include "timeout.h"
#include "utils_math.h"

// DJI-style CAN protocol constants.
#define CAN_DJI_CTRL_ID_1_TO_4			0x200
#define CAN_DJI_CTRL_ID_5_TO_8			0x1FF
#define CAN_DJI_STATUS_ID_BASE			0x200
#define CAN_DJI_CURRENT_SCALE_A			20.0f
#define CAN_DJI_CURRENT_SCALE_RAW		16384.0f
#define CAN_DJI_ANGLE_SCALE_RAW			8192.0f

// Threads
static THD_WORKING_AREA(status_thread_wa, 512);
static THD_FUNCTION(status_thread, arg);

// Private functions
static bool can_dji_sid_callback(uint32_t sid, uint8_t *data, uint8_t len);
static void send_status(void);
static bool id_is_valid(int id);

// Private variables
static volatile bool stop_now = true;
static volatile bool is_running = false;

void app_custom_start(void) {
	stop_now = false;

	comm_can_set_sid_rx_callback(can_dji_sid_callback);

	chThdCreateStatic(status_thread_wa, sizeof(status_thread_wa),
			NORMALPRIO, status_thread, NULL);
}

void app_custom_stop(void) {
	comm_can_set_sid_rx_callback(0);

	stop_now = true;
	mc_interface_release_motor();

	while (is_running) {
		chThdSleepMilliseconds(1);
	}
}

void app_custom_configure(app_configuration *conf) {
	(void)conf;
}

static bool can_dji_sid_callback(uint32_t sid, uint8_t *data, uint8_t len) {
	const app_configuration *conf = app_get_configuration();
	int id = conf->controller_id;

	if (conf->app_to_use != APP_CUSTOM || !id_is_valid(id) || len < 8) {
		return false;
	}

	uint32_t expected_sid = id <= 4 ? CAN_DJI_CTRL_ID_1_TO_4 : CAN_DJI_CTRL_ID_5_TO_8;
	if (sid != expected_sid) {
		return false;
	}

	int offset = ((id - 1) % 4) * 2;
	int16_t current_raw = (int16_t)(((uint16_t)data[offset] << 8) | data[offset + 1]);
	float current = (float)current_raw * CAN_DJI_CURRENT_SCALE_A / CAN_DJI_CURRENT_SCALE_RAW;

	mc_interface_set_current(current);
	timeout_reset();

	return true;
}

static THD_FUNCTION(status_thread, arg) {
	(void)arg;

	chRegSetThreadName("DJI CAN");
	is_running = true;

	while (!stop_now) {
		const app_configuration *conf = app_get_configuration();

		if (conf->app_to_use == APP_CUSTOM && id_is_valid(conf->controller_id) &&
				conf->can_status_rate_1 > 0) {
			send_status();

			systime_t sleep_time = CH_CFG_ST_FREQUENCY / conf->can_status_rate_1;
			if (sleep_time == 0) {
				sleep_time = 1;
			}

			chThdSleep(sleep_time);
		} else {
			chThdSleepMilliseconds(10);
		}
	}

	is_running = false;
}

static void send_status(void) {
	const app_configuration *appconf = app_get_configuration();
	int id = appconf->controller_id;

	if (!id_is_valid(id)) {
		return;
	}

	const volatile mc_configuration *mcconf = mc_interface_get_configuration();

	float angle = mc_interface_get_pid_pos_now();
	utils_norm_angle(&angle);
	if (angle >= 360.0f) {
		angle = 0.0f;
	}

	int angle_raw = (int)(angle * CAN_DJI_ANGLE_SCALE_RAW / 360.0f);
	utils_truncate_number_int(&angle_raw, 0, 8191);

	float pole_pairs = (float)mcconf->si_motor_poles / 2.0f;
	if (pole_pairs < 1.0f) {
		pole_pairs = 1.0f;
	}

	int rpm_raw = (int)(mc_interface_get_rpm() / pole_pairs);
	utils_truncate_number_int(&rpm_raw, -32768, 32767);

	int current_raw = (int)(mc_interface_get_tot_current_directional_filtered() *
			CAN_DJI_CURRENT_SCALE_RAW / CAN_DJI_CURRENT_SCALE_A);
	utils_truncate_number_int(&current_raw, -32768, 32767);

	float temp_motor = mc_interface_temp_motor_filtered();
	float temp_mos = mc_interface_temp_fet_filtered();
	int temp = (int)(temp_mos > temp_motor ? temp_mos : temp_motor);
	utils_truncate_number_int(&temp, -128, 127);

	uint8_t buffer[8];
	buffer[0] = (uint8_t)((uint16_t)angle_raw >> 8);
	buffer[1] = (uint8_t)((uint16_t)angle_raw);
	buffer[2] = (uint8_t)((uint16_t)(int16_t)rpm_raw >> 8);
	buffer[3] = (uint8_t)((uint16_t)(int16_t)rpm_raw);
	buffer[4] = (uint8_t)((uint16_t)(int16_t)current_raw >> 8);
	buffer[5] = (uint8_t)((uint16_t)(int16_t)current_raw);
	buffer[6] = (uint8_t)(int8_t)temp;
	buffer[7] = (uint8_t)mc_interface_get_fault();

	comm_can_transmit_sid(CAN_DJI_STATUS_ID_BASE + id, buffer, 8);
}

static bool id_is_valid(int id) {
	return id >= 1 && id <= 8;
}
