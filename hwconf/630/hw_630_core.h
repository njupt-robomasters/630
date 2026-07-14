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

#ifndef HW_630_CORE_H_
#define HW_630_CORE_H_

#define APP_CUSTOM_TO_USE	"app_custom_can_dji.c"

// HW properties
#define HW_HAS_3_SHUNTS
#define HW_HAS_PHASE_SHUNTS

// ADC macros and settings
/*
 * ADC Vector
 *
 * 0:	IN0		SENS1
 * 1:	IN1		SENS2
 * 2:	IN2		SENS3
 * 3:	IN10	CURR1
 * 4:	IN11	CURR2
 * 5:	IN12	CURR3
 * 6:	IN5		ADC_EXT1
 * 7:	IN6		ADC_EXT2
 * 8:	IN3		TEMP_PCB
 * 9:	IN14	TEMP_MOTOR
 * 10:	IN15	ADC_EXT3, Shutdown on MK3
 * 11:	IN13	AN_IN
 * 12:	Vrefint
 * 13:	IN0		SENS1
 * 14:	IN1		SENS2
 */
#define HW_ADC_INJ_CHANNELS		3
#define HW_ADC_NBR_CONV			5
#define HW_ADC_CHANNELS			(HW_ADC_NBR_CONV * 3)
// ADC Indexes
#define ADC_IND_SENS1			3
#define ADC_IND_SENS2			4
#define ADC_IND_SENS3			5
#define ADC_IND_CURR1			0
#define ADC_IND_CURR2			1
#define ADC_IND_CURR3			2
#define ADC_IND_VIN_SENS		11
#define ADC_IND_EXT				6
#define ADC_IND_EXT2			7
#define ADC_IND_TEMP_MOS		8
#define ADC_IND_TEMP_MOTOR		9
#define ADC_IND_VREFINT			12
// COMM-port ADC GPIOs
#define HW_ADC_EXT_GPIO			GPIOA
#define HW_ADC_EXT_PIN			5
#define HW_ADC_EXT2_GPIO		GPIOA
#define HW_ADC_EXT2_PIN			6
// Component parameters
#define V_REG					3.3
#define VIN_R1					39000.0
#define VIN_R2					2200.0
// Input voltage
#define GET_INPUT_VOLTAGE()		((V_REG / 4095.0) * (float)ADC_Value[ADC_IND_VIN_SENS] * ((VIN_R1 + VIN_R2) / VIN_R2))
// NTC Termistors
#define NTC_RES(adc_val)		((4095.0 * 10000.0) / adc_val - 10000.0)
#define NTC_TEMP(adc_ind)		(1.0 / ((logf(NTC_RES(ADC_Value[adc_ind]) / 10000.0) / 3380.0) + (1.0 / 298.15)) - 273.15)
#define NTC_RES_MOTOR(adc_val)	(10000.0 / ((4095.0 / (float)adc_val) - 1.0)) // Motor temp sensor on low side
#define NTC_TEMP_MOTOR(beta)	(1.0 / ((logf(NTC_RES_MOTOR(ADC_Value[ADC_IND_TEMP_MOTOR]) / 10000.0) / beta) + (1.0 / 298.15)) - 273.15)
// Voltage on ADC channel
#define ADC_VOLTS(ch)			((float)ADC_Value[ch] / 4096.0 * V_REG)
// Measurement macros
#define ADC_V_L1				ADC_Value[ADC_IND_SENS1]
#define ADC_V_L2				ADC_Value[ADC_IND_SENS2]
#define ADC_V_L3				ADC_Value[ADC_IND_SENS3]
#define ADC_V_ZERO				(ADC_Value[ADC_IND_VIN_SENS] / 2)
// Double samples in beginning and end for positive current measurement.
// Useful when the shunt sense traces have noise that causes offset.
#define CURR1_DOUBLE_SAMPLE		0
#define CURR2_DOUBLE_SAMPLE		0
#define CURR3_DOUBLE_SAMPLE		0

// Hall/encoder pins
#define HW_HALL_ENC_GPIO1		GPIOC
#define HW_HALL_ENC_PIN1		6
#define HW_HALL_ENC_GPIO2		GPIOC
#define HW_HALL_ENC_PIN2		7
#define HW_HALL_ENC_GPIO3		GPIOC
#define HW_HALL_ENC_PIN3		8
#define HW_ENC_TIM				TIM3
#define HW_ENC_TIM_AF			GPIO_AF_TIM3
#define HW_ENC_TIM_CLK_EN()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE)
#define HW_ENC_EXTI_PORTSRC		EXTI_PortSourceGPIOC
#define HW_ENC_EXTI_PINSRC		EXTI_PinSource8
#define HW_ENC_EXTI_CH			EXTI9_5_IRQn
#define HW_ENC_EXTI_LINE		EXTI_Line8
#define HW_ENC_EXTI_ISR_VEC		EXTI9_5_IRQHandler
#define HW_ENC_TIM_ISR_CH		TIM3_IRQn
#define HW_ENC_TIM_ISR_VEC		TIM3_IRQHandler
// Hall Macros
#define READ_HALL1()			palReadPad(HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1)
#define READ_HALL2()			palReadPad(HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2)
#define READ_HALL3()			palReadPad(HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3)

// SPI pins
#define HW_SPI_DEV				SPID1
#define HW_SPI_GPIO_AF			GPIO_AF_SPI1
#define HW_SPI_PORT_NSS			GPIOA
#define HW_SPI_PIN_NSS			4
#define HW_SPI_PORT_SCK			GPIOA
#define HW_SPI_PIN_SCK			5
#define HW_SPI_PORT_MOSI		GPIOA
#define HW_SPI_PIN_MOSI			7
#define HW_SPI_PORT_MISO		GPIOA
#define HW_SPI_PIN_MISO			6

// I2C Peripheral
#define HW_I2C_DEV				I2CD2
#define HW_I2C_GPIO_AF			GPIO_AF_I2C2
#define HW_I2C_SCL_PORT			GPIOB
#define HW_I2C_SCL_PIN			10
#define HW_I2C_SDA_PORT			GPIOB
#define HW_I2C_SDA_PIN			11

// UART Peripheral
#define HW_UART_DEV				SD3
#define HW_UART_GPIO_AF			GPIO_AF_USART3
#define HW_UART_TX_PORT			GPIOB
#define HW_UART_TX_PIN			10
#define HW_UART_RX_PORT			GPIOB
#define HW_UART_RX_PIN			11

// ICU Peripheral for servo decoding
#define HW_USE_SERVO_TIM4
#define HW_ICU_TIMER			TIM4
#define HW_ICU_TIM_CLK_EN()		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE)
#define HW_ICU_DEV				ICUD4
#define HW_ICU_CHANNEL			ICU_CHANNEL_1
#define HW_ICU_GPIO_AF			GPIO_AF_TIM4
#define HW_ICU_GPIO				GPIOB
#define HW_ICU_PIN				6

// Setting limits
#define HW_LIM_CURRENT			-40.0, 40.0
#define HW_LIM_CURRENT_IN		-40.0, 40.0
#define HW_LIM_CURRENT_ABS		0.0, 40.0
#define HW_LIM_VIN				6.0, 57.0
#define HW_LIM_ERPM				-1e6, 1e6
#define HW_LIM_DUTY_MIN			0.0, 0.1
#define HW_LIM_DUTY_MAX			0.0, 0.99
#define HW_LIM_TEMP_FET			-40.0, 110.0

// Default setting overrides
// Limits
#define MCCONF_L_CURRENT_MAX			20.0	// Current limit in Amperes (Upper)
#define MCCONF_L_CURRENT_MIN			-20.0	// Current limit in Amperes (Lower)
#define MCCONF_L_IN_CURRENT_MAX			30.0	// Input current limit in Amperes (Upper)
#define MCCONF_L_IN_CURRENT_MIN			-30.0	// Input current limit in Amperes (Lower)
#define MCCONF_L_MAX_ABS_CURRENT		30.0	// The maximum absolute current above which a fault is generated
#define MCCONF_L_RPM_MAX				1e6	// The motor speed limit (Upper)
#define MCCONF_L_RPM_MIN				-1e6	// The motor speed limit (Lower)
// Sensors
#define MCCONF_M_SENSOR_PORT_MODE		SENSOR_PORT_MODE_SINCOS // The mode of the sincos_encoder port
#define MCCONF_M_ENCODER_SIN_AMP		0.8	// Sine amplitude
#define MCCONF_M_ENCODER_SIN_OFFSET		1.0 // Sine offset of the sin/cos encoder
#define MCCONF_M_ENCODER_COS_AMP		0.8	// Cosine amplitude
#define MCCONF_M_ENCODER_COS_OFFSET		1.0 // Cosine offset of the sin/cos encoder
// FOC
#define MCCONF_FOC_SENSOR_MODE			FOC_SENSOR_MODE_HFI
#define MCCONF_FOC_MOTOR_L				4.85e-05
#define MCCONF_FOC_MOTOR_R				0.097
#define MCCONF_FOC_MOTOR_LD_LQ_DIFF		1.3e-05
#define MCCONF_FOC_MOTOR_FLUX_LINKAGE	0.0015
#define MCCONF_FOC_CURRENT_KP			0.097
#define MCCONF_FOC_CURRENT_KI			194.0
#define MCCONF_FOC_OBSERVER_GAIN		4.4444e+08		// Can be something like 600 / L
#define MCCONF_FOC_ENCODER_RATIO		1.0
#define MCCONF_FOC_HFI_AMB_CURRENT		30.0 // HFI ambiguity resolution current
#define MCCONF_FOC_OFFSETS_CAL_MODE		4 // Offset calibration mode
#define MCCONF_FOC_F_ZV					30000.0
// Speed PID parameters
#define MCCONF_S_PID_KP					0.02	// Proportional gain
#define MCCONF_S_PID_KI					0.1	// Integral gain
#define MCCONF_S_PID_KD					0.0005	// Derivative gain
#define MCCONF_S_PID_MIN_RPM			0.0	// Minimum allowed RPM
#define MCCONF_S_PID_RAMP_ERPMS_S		-1.00	// Speed input ramping, in ERPM/s
// Position PID parameters
#define MCCONF_P_PID_KP					0.005	// Proportional gain
#define MCCONF_P_PID_ANG_DIV			7.0		// Divide angle by this value
// App configuration
#define APPCONF_APP_TO_USE					APP_CUSTOM
#define APPCONF_CONTROLLER_ID				1 // Controller id. -1 means it should be calculated from UUID.
#define APPCONF_TIMEOUT_MSEC				500
#define APPCONF_CAN_BAUD_RATE				CAN_BAUD_1M
#define APPCONF_CAN_STATUS_RATE_1			1000
#define APPCONF_CAN_STATUS_RATE_2			1000

#endif /* HW_630_CORE_H_ */
