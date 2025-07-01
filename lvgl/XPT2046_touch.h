/*
 * XPT2046_touch.h
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */

#ifndef XPT2046_TOUCH_H_
#define XPT2046_TOUCH_H_

#include "main.h"
#include <stdbool.h>

/*** Redefine if necessary ***/

// Warning! Use SPI bus with < 2.5 Mbit speed, better ~650 Kbit to be save.
#define XPT2046_SPI_PORT hspi2
extern SPI_HandleTypeDef XPT2046_SPI_PORT;

#define XPT2046_IRQ_Pin       T_IRQ_Pin
#define XPT2046_IRQ_GPIO_Port T_IRQ_GPIO_Port
#define XPT2046_CS_Pin        T_CS_Pin
#define XPT2046_CS_GPIO_Port  T_CS_GPIO_Port

// change depending on screen orientation
#define XPT2046_SCALE_X 320
#define XPT2046_SCALE_Y 240

#define XPT2046_MIN_RAW_X 1860
#define XPT2046_MAX_RAW_X 29650
#define XPT2046_MIN_RAW_Y 1830
#define XPT2046_MAX_RAW_Y 29350

//#define XPT2046_MIN_RAW_X 2000
//#define XPT2046_MAX_RAW_X 30000
//#define XPT2046_MIN_RAW_Y 1500
//#define XPT2046_MAX_RAW_Y 29000

#define XTP2046_CALI_DIFF	2500

// call before initializing any SPI devices
void XPT2046_TouchUnselect(void);
bool XPT2046_TouchPressed(void);
bool XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y);
bool XPT2046_TouchGetRawCoordinates(uint32_t* raw_, uint32_t* raw_y);
bool XPT2046_TouchCalibration(void);

#endif /* XPT2046_TOUCH_H_ */

