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

#ifndef HW_P630_H_
#define HW_P630_H_

#define HW_NAME					"P630"
#define HW_MAJOR				0
#define HW_MINOR				2

#define CURRENT_AMP_GAIN		20.0
#define CURRENT_SHUNT_RES		0.002

// LED Macros
#define LED_GREEN_ON()			palClearPad(GPIOB, 0)
#define LED_GREEN_OFF()			palSetPad(GPIOB, 0)
#define LED_RED_ON()			palClearPad(GPIOB, 1)
#define LED_RED_OFF()			palSetPad(GPIOB, 1)

#include "hw_630_core.h"

#endif /* HW_P630_H_ */
