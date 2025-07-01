/*
 * lcd_fsmc.c
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */


#include <lcd_fsmc.h>
#include <stdlib.h>
#include <stdio.h>


static uint8_t LCD_Orientation=0;
static lv_coord_t lcd_width = LCD_WIDTH; //default 240
static lv_coord_t lcd_height = LCD_HEIGHT; //default 320

// MADCTL register: 			MY,MX,MV,ML,BGR,MH,x,x
static uint8_t LCD_MADCTL_PORTRAIT  		=	0b01001000;
static uint8_t LCD_MADCTL_LANDSCAPE  		=	0b00101000;
static uint8_t LCD_MADCTL_PORTRAIT_MIRROR  =	0b10001000;
static uint8_t LCD_MADCTL_LANDSCAPE_MIRROR = 	0b11101000;

void lcd_cmd_write(unsigned char command)
{
	LCD_CMD_WRITE(command);
}

void lcd_data_write(unsigned short data)
{
	LCD_DATA_WRITE(data);
}

lv_coord_t lcd_get_width() {
	return lcd_width;
}

lv_coord_t lcd_get_height() {
	return lcd_height;
}

static void lcd_reset(void)
{
	lcd_cmd_write(LCD_SOFTRESET);
	HAL_Delay(50);
}

void lcd_set_window(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1)
{
  lcd_cmd_write(LCD_COLADDRSET);
  lcd_data_write((x0 >> 8) & 0xFF);
  lcd_data_write(x0 & 0xFF);
  lcd_data_write((x1 >> 8) & 0xFF);
  lcd_data_write(x1 & 0xFF);
  lcd_cmd_write(LCD_PAGEADDRSET);
  lcd_data_write((y0 >> 8) & 0xFF);
  lcd_data_write(y0 & 0xFF);
  lcd_data_write((y1 >> 8) & 0xFF);
  lcd_data_write(y1 & 0xFF);
  lcd_cmd_write(LCD_MEMORYWRITE);
}

void lcd_init(void)
{
  lcd_reset();

  lcd_cmd_write(LCD_DISPLAYOFF);

  lcd_cmd_write(0xCF);
  lcd_data_write(0x00);
  lcd_data_write(0x83);
  lcd_data_write(0x30);

  lcd_cmd_write(0xED);
  lcd_data_write(0x64);
  lcd_data_write(0x03);
  lcd_data_write(0x12);
  lcd_data_write(0x81);

  lcd_cmd_write(0xE8);
  lcd_data_write(0x85);
  lcd_data_write(0x01);
  lcd_data_write(0x79);

  lcd_cmd_write(0xCB);
  lcd_data_write(0x39);
  lcd_data_write(0x2C);
  lcd_data_write(0x00);
  lcd_data_write(0x34);
  lcd_data_write(0x02);

  lcd_cmd_write(0xF7);
  lcd_data_write(0x20);

  lcd_cmd_write(0xEA);
  lcd_data_write(0x00);
  lcd_data_write(0x00);

  lcd_cmd_write(LCD_POWERCONTROL1);
  lcd_data_write(0x26);

  lcd_cmd_write(LCD_POWERCONTROL2);
  lcd_data_write(0x11);

  lcd_cmd_write(LCD_VCOMCONTROL1);
  lcd_data_write(0x35);
  lcd_data_write(0x3E);

  lcd_cmd_write(LCD_VCOMCONTROL2);
  lcd_data_write(0xBE);

  lcd_cmd_write(LCD_MEMCONTROL);
  lcd_data_write(LCD_MADCTL_PORTRAIT);
  LCD_Orientation = LCD_ORIENTATION_PORTRAIT; // set TFT orientation default

  lcd_cmd_write(LCD_PIXELFORMAT);
  lcd_data_write(0x55);

  lcd_cmd_write(LCD_FRAMECONTROLNORMAL);
  lcd_data_write(0x00);
  lcd_data_write(0x1B);

  lcd_cmd_write(0xF2);
  lcd_data_write(0x08);

  lcd_cmd_write(LCD_GAMMASET);
  lcd_data_write(0x01);

  lcd_cmd_write(LCD_POSITIVEGAMMCORR);
  lcd_data_write(0x1F);
  lcd_data_write(0x1A);
  lcd_data_write(0x18);
  lcd_data_write(0x0A);
  lcd_data_write(0x0F);
  lcd_data_write(0x06);
  lcd_data_write(0x45);
  lcd_data_write(0x87);
  lcd_data_write(0x32);
  lcd_data_write(0x0A);
  lcd_data_write(0x07);
  lcd_data_write(0x02);
  lcd_data_write(0x07);
  lcd_data_write(0x05);
  lcd_data_write(0x00);

  lcd_cmd_write(LCD_NEGATIVEGAMMCORR);
  lcd_data_write(0x00);
  lcd_data_write(0x25);
  lcd_data_write(0x27);
  lcd_data_write(0x05);
  lcd_data_write(0x10);
  lcd_data_write(0x09);
  lcd_data_write(0x3A);
  lcd_data_write(0x78);
  lcd_data_write(0x4D);
  lcd_data_write(0x05);
  lcd_data_write(0x18);
  lcd_data_write(0x0D);
  lcd_data_write(0x38);
  lcd_data_write(0x3A);
  lcd_data_write(0x1F);

  lcd_cmd_write(LCD_COLADDRSET);
  lcd_data_write(0x00);
  lcd_data_write(0x00);
  lcd_data_write(0x00);
  lcd_data_write(0xEF);

  lcd_cmd_write(LCD_PAGEADDRSET);
  lcd_data_write(0x00);
  lcd_data_write(0x00);
  lcd_data_write(0x01);
  lcd_data_write(0x3F);

  lcd_cmd_write(LCD_ENTRYMODE);
  lcd_data_write(0x07);

  lcd_cmd_write(LCD_DISPLAYFUNC);
  lcd_data_write(0x0A);
  lcd_data_write(0x82);
  lcd_data_write(0x27);
  lcd_data_write(0x00);

  lcd_cmd_write(LCD_SLEEPOUT);
  HAL_Delay(100);
  lcd_cmd_write(LCD_DISPLAYON);
  HAL_Delay(100);
  lcd_cmd_write(LCD_MEMORYWRITE);
}

void lcd_set_orientation(uint8_t Orientation)
{
	LCD_Orientation = Orientation;
	lcd_cmd_write(LCD_MEMCONTROL);

	switch (LCD_Orientation)
	{
		case LCD_ORIENTATION_PORTRAIT:
			lcd_data_write(LCD_MADCTL_PORTRAIT);
			lcd_width = LCD_WIDTH;
			lcd_height = LCD_HEIGHT;
			break;
		case LCD_ORIENTATION_PORTRAIT_MIRROR:
			lcd_data_write(LCD_MADCTL_PORTRAIT_MIRROR);
			lcd_width = LCD_WIDTH;
			lcd_height = LCD_HEIGHT;
			break;
		case LCD_ORIENTATION_LANDSCAPE:
			lcd_data_write(LCD_MADCTL_LANDSCAPE);
			lcd_width = LCD_HEIGHT;
			lcd_height = LCD_WIDTH;
			break;
		case LCD_ORIENTATION_LANDSCAPE_MIRROR:
			lcd_data_write(LCD_MADCTL_LANDSCAPE_MIRROR);
			lcd_width = LCD_HEIGHT;
			lcd_height = LCD_WIDTH;
			break;
		default:
			break;
	}

	lcd_cmd_write(LCD_MEMORYWRITE);
	lcd_set_window(0, 0, lcd_width - 1, lcd_height - 1);
}

void lcd_backlight_off(void)
{
	LCD_BL_OFF();
}

void lcd_backlight_on(void)
{
	LCD_BL_ON();
}

void lcd_display_off(void)
{
	lcd_cmd_write(LCD_DISPLAYOFF);
	LCD_BL_OFF();
}

void lcd_display_on(void)
{
	lcd_cmd_write(LCD_DISPLAYON);
	LCD_BL_ON();
}

uint8_t lcd_get_orientation(void)
{
  return LCD_Orientation;
}


void lcd_fill_RGB(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
  lcd_set_window(x, y, x+width - 1, y+height - 1);
  int dimensions = width * height;
  while(dimensions--)
  {
    lcd_data_write(color);
  }
}
