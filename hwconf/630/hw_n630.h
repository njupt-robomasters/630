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

#ifndef HW_N630_H_
#define HW_N630_H_

#define HW_NAME					"N630"
#define HW_MAJOR				6
#define HW_MINOR				0

#define CURRENT_AMP_GAIN		50.0
#define CURRENT_SHUNT_RES		0.001

#define HW_HAS_DRV8301
// SPI for DRV8301
#define DRV8301_MOSI_GPIO		GPIOC
#define DRV8301_MOSI_PIN		12
#define DRV8301_MISO_GPIO		GPIOC
#define DRV8301_MISO_PIN		11
#define DRV8301_SCK_GPIO		GPIOC
#define DRV8301_SCK_PIN			10
#define DRV8301_CS_GPIO			GPIOC
#define DRV8301_CS_PIN			9

#include "hw_630_core.h"

#endif /* HW_N630_H_ */
