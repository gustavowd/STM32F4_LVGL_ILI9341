/*
 * XPT2046_touch.c
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */

#include <lcd_fsmc.h>
#include <stdio.h>
#include <stdlib.h>
#include "XPT2046_touch.h"
#include "FreeRTOS.h"
#include "task.h"


#define READ_X 0x90
#define READ_Y 0xD0


uint16_t cRawX_min = XPT2046_MIN_RAW_X;
uint16_t cRawX_max = XPT2046_MAX_RAW_X;
uint16_t cRawY_min = XPT2046_MIN_RAW_Y;
uint16_t cRawY_max = XPT2046_MAX_RAW_Y;


static void XPT2046_TouchSelect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_RESET);
}

void XPT2046_TouchUnselect()
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, GPIO_PIN_SET);
}

bool XPT2046_TouchPressed()
{
    return HAL_GPIO_ReadPin(XPT2046_IRQ_GPIO_Port, XPT2046_IRQ_Pin) == GPIO_PIN_RESET;
}

bool XPT2046_TouchGetCoordinates(uint16_t* x, uint16_t* y)
{
	bool ret_value=false;
	uint16_t tx,ty;
	uint32_t raw_x;
	uint32_t raw_y;
	if (XPT2046_TouchGetRawCoordinates(&raw_x, &raw_y))
	{
		if(raw_x < cRawX_min) raw_x = cRawX_min;
		if(raw_x > cRawX_max) raw_x = cRawX_max;

		if(raw_y < cRawY_min) raw_y = cRawY_min;
		if(raw_y > cRawY_max) raw_y = cRawY_max;

		tx = (raw_x - cRawX_min) * XPT2046_SCALE_X / (cRawX_max - cRawX_min);
		ty = (raw_y - cRawY_min) * XPT2046_SCALE_Y / (cRawY_max - cRawY_min);

		/*
		uint8_t lot = lcd_get_orientation();
		switch (lot)
		{
		case LCD_ORIENTATION_PORTRAIT:
			*x=tx;
			*y=ty;
			break;
		case LCD_ORIENTATION_LANDSCAPE:
			*x=ty;
			*y=LCD_WIDTH-tx;
					break;
		case LCD_ORIENTATION_PORTRAIT_MIRROR:
			*x=LCD_WIDTH-tx;
			*y=LCD_HEIGHT-ty;
					break;
		case LCD_ORIENTATION_LANDSCAPE_MIRROR:
			*x=LCD_HEIGHT-ty;
			*y=tx;
			break;
		}
		*/
		*x=tx;
		*y=ty;
		ret_value =true;
    }
	return ret_value;

}
bool XPT2046_TouchGetRawCoordinates(uint32_t* raw_x, uint32_t* raw_y)
{
    static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };
    static const uint8_t SAMPLES=16;

    //if (!XPT2046_TouchPressed()) return false;
    //vTaskDelay(10);
    XPT2046_TouchSelect();

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
    uint8_t nsamples = 0;


    for(uint8_t i = 0; i < SAMPLES; i++)
    {
        if(!XPT2046_TouchPressed()) {
            break;
        }

        nsamples++;

        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
        uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(&XPT2046_SPI_PORT, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
        uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&XPT2046_SPI_PORT, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);

        avg_x += (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
        avg_y += (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
    }

    XPT2046_TouchUnselect();

    if(nsamples < SAMPLES)
        return false;

    *raw_x = (avg_x / SAMPLES);
    *raw_y = (avg_y / SAMPLES);
    return true;

}

bool XPT2046_TouchCalibration()
{
	uint32_t x0=0,y0=0,x1=0,y1=0,x2=0,y2=0,x3=0,y3=0;
	bool correct=true;
	uint32_t width, height;
	uint8_t lot = lcd_get_orientation();
	lcd_set_orientation(LCD_ORIENTATION_LANDSCAPE);
	width = lcd_get_width();
	height = lcd_get_height();
	lcd_fill_RGB(0x0000, 0, 0, width-1, height-1);



	lcd_fill_RGB(0xffff, 0, 0, 6,6);
	lcd_set_window(20,100 ,20 ,100);  // set LCD cursor to (20,100)
	while(!XPT2046_TouchPressed()) ;
	if (!XPT2046_TouchGetRawCoordinates(&x0, &y0))
	{
		lcd_set_orientation(lot);
		return false;
	}

	lcd_fill_RGB(0x0000, 0, 0, width-1, height-1);
	lcd_fill_RGB(0xffff, 0, height-7, 6, 6);

	while(XPT2046_TouchPressed());
	vTaskDelay(500);

	lcd_set_window(20, 100, 20, 100);
	while(!XPT2046_TouchPressed());
	if(!XPT2046_TouchGetRawCoordinates(&x1, &y1))
	{
		lcd_set_orientation(lot);
		return false;
	}
	lcd_fill_RGB(0x0000, 0, 0, width-1, height-1);
	lcd_fill_RGB(0xffff,width-7, height-7, 6, 6);
	while(XPT2046_TouchPressed());
	vTaskDelay(500);


	lcd_set_window(20, 100, 20, 100);
	while(!XPT2046_TouchPressed());
	if (!XPT2046_TouchGetRawCoordinates(&x2, &y2))
	{
		lcd_set_orientation(lot);
		return false;
	}
	lcd_fill_RGB(0x0000, 0, 0, width-1, height-1);
	lcd_fill_RGB(0xffff, width-7, 0, 6, 6);
	while(XPT2046_TouchPressed());
	vTaskDelay(500);

	lcd_set_window(20, 100, 20, 100);
	while(!XPT2046_TouchPressed());
	if (!XPT2046_TouchGetRawCoordinates(&x3, &y3))
	{
		lcd_set_orientation(lot);
		return false;
	}
	while(XPT2046_TouchPressed());

	if (abs(x0-x1) > XTP2046_CALI_DIFF) correct = false;
	if (abs(x2-x3) > XTP2046_CALI_DIFF) correct = false;
	if (abs(y1-y2) > XTP2046_CALI_DIFF) correct = false;
	if (abs(y0-y3) > XTP2046_CALI_DIFF) correct = false;
	if (correct) {
		cRawX_min = (x0+x1)/2;
		cRawX_max = (x2+x3)/2;
		cRawY_min = (y0+y3)/2;
		cRawY_max = (y1+y2)/2;
	}

	lcd_fill_RGB(0x0000, 0, 0, width-1, height-1);
	lcd_set_window(20, 100, 20, 100);
	lcd_set_orientation(lot);
	return correct;
}


