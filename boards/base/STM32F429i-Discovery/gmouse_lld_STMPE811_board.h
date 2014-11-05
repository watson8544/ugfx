/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GINPUT_LLD_MOUSE_BOARD_H
#define _GINPUT_LLD_MOUSE_BOARD_H

// Resolution and Accuracy Settings
#define GMOUSE_STMPE811_PEN_CALIBRATE_ERROR		8
#define GMOUSE_STMPE811_PEN_CLICK_ERROR			6
#define GMOUSE_STMPE811_PEN_MOVE_ERROR			4
#define GMOUSE_STMPE811_FINGER_CALIBRATE_ERROR	14
#define GMOUSE_STMPE811_FINGER_CLICK_ERROR		18
#define GMOUSE_STMPE811_FINGER_MOVE_ERROR		14

// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_STMPE811_BOARD_DATA_SIZE			0

// Set this to TRUE if you want self-calibration.
//	NOTE:	This is not as accurate as real calibration.
//			It requires the orientation of the touch panel to match the display.
//			It requires the active area of the touch panel to exactly match the display size.
#define GMOUSE_STMPE811_SELF_CALIBRATE			FALSE

// If TRUE this board has the STMPE811 IRQ pin connected to a GPIO.
#define GMOUSE_STMPE811_GPIO_IRQPIN				TRUE

// If TRUE this is a really slow CPU and we should always clear the FIFO between reads.
#define GMOUSE_STMPE811_SLOW_CPU				FALSE

// Maximum timeout
#define STMPE811_TIMEOUT			0x3000

static const I2CConfig i2ccfg = {
	OPMODE_I2C,
	400000,
	FAST_DUTY_CYCLE_2,
};

static bool_t init_board(GMouse* m, unsigned driverinstance) {
	(void)		m;

	// This board only supports one touch panel
	if (driverinstance)
		return FALSE;

	palSetPadMode(GPIOA, 15, PAL_MODE_INPUT | PAL_STM32_PUDR_FLOATING);			/* TP IRQ */
	palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);	/* SCL */
	palSetPadMode(GPIOC, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);	/* SDA */

	i2cStart(&I2CD3, &i2ccfg);

	return TRUE;
}

#if GMOUSE_STMPE811_GPIO_IRQPIN
	static bool_t getpin_irq(GMouse* m) {
		(void)		m;

		return !palReadPad(GPIOA, 15);
	}
#endif

static inline void aquire_bus(GMouse* m) {
	(void)		m;

}

static inline void release_bus(GMouse* m) {
	(void)		m;

}

static void write_reg(GMouse* m, uint8_t reg, uint8_t val) {
	uint8_t		txbuf[2];
	(void)		m;

	txbuf[0] = reg;
	txbuf[1] = val;

	i2cAcquireBus(&I2CD3);
	i2cMasterTransmitTimeout(&I2CD3, STMPE811_ADDR, txbuf, 2, 0, 0, MS2ST(STMPE811_TIMEOUT));
	i2cReleaseBus(&I2CD3);
}

static uint8_t read_byte(GMouse* m, uint8_t reg) {
	uint8_t		rxbuf[1];
	(void)		m;

	rxbuf[0] = 0;

	i2cAcquireBus(&I2CD3);
	i2cMasterTransmitTimeout(&I2CD3, STMPE811_ADDR, &reg, 1, rxbuf, 1, MS2ST(STMPE811_TIMEOUT));
	i2cReleaseBus(&I2CD3);

	return rxbuf[0];
}

static uint16_t read_word(GMouse* m, uint8_t reg) {
	uint8_t		rxbuf[2];
	(void)		m;

	rxbuf[0] = 0;
	rxbuf[1] = 0;

	i2cAcquireBus(&I2CD3);
	i2cMasterTransmitTimeout(&I2CD3, STMPE811_ADDR, &reg, 1, rxbuf, 2, MS2ST(STMPE811_TIMEOUT));
	i2cReleaseBus(&I2CD3);

	return (((uint16_t)rxbuf[0]) << 8) | rxbuf[1];
}

#endif /* _GINPUT_LLD_MOUSE_BOARD_H */
