#ifndef __LCD_FSMC_H_
#define __LCD_FSMC_H_

#include "main.h"
#include <stdbool.h>
#include "lvgl.h"

#define LCD_WIDTH		240
#define LCD_HEIGHT 		320

#define LCD_BL_ON() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET)
#define LCD_BL_OFF() HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET)

#define LCD_CMD_BASE        		((uint32_t)0x60000000)
#define LCD_DATA_BASE        		((uint32_t)0x60080000)

#define LCD_CMD_WRITE(command)	*(volatile uint16_t *) (LCD_CMD_BASE) = (command)
#define LCD_DATA_WRITE(data)	*(volatile uint16_t *) (LCD_DATA_BASE) = (data)
#define	LCD_REGISTER_READ()		*(volatile uint16_t *) (LCD_CMD_BASE)
#define	LCD_DATA_READ()			*(volatile uint16_t *) (LCD_DATA_BASE)

enum
{
	LCD_ORIENTATION_PORTRAIT 			= 0,
	LCD_ORIENTATION_LANDSCAPE 			= 1,
	LCD_ORIENTATION_PORTRAIT_MIRROR 	= 2,
	LCD_ORIENTATION_LANDSCAPE_MIRROR 	= 3
};

// LCD registers
#define	LCD_NOP					0x00
#define LCD_SOFTRESET			0x01
#define LCD_READID				0x04
#define LCD_READSTATUS			0x09
#define LCD_READPOWERMODE		0x0A
#define LCD_READMADCTL			0x0B
#define LCD_READPIXELFORMAT		0x0C
#define LCD_READIMAGEFORMAT		0x0D
#define LCD_READSIGNALMODE		0x0E
#define LCD_READSELFDIAGNOSTIC	0x0F
#define LCD_SLEEPIN				0x10
#define LCD_SLEEPOUT			0x11
#define LCD_PARTIALMODE			0x12
#define LCD_NORMALDISP			0x13
#define LCD_INVERTOFF			0x20
#define LCD_INVERTON			0x21
#define LCD_GAMMASET			0x26
#define LCD_DISPLAYOFF			0x28
#define LCD_DISPLAYON			0x29
#define LCD_COLADDRSET			0x2A
#define LCD_PAGEADDRSET			0x2B
#define LCD_MEMORYWRITE			0x2C
#define LCD_COLORSET			0x2D
#define LCD_MEMORYREAD			0x2E
#define LCD_PARTIALAREA			0x30
#define LCD_VERTICALSCROLING	0x33
#define LCD_TEARINGEFFECTOFF	0x34
#define LCD_TEARINGEFFECTON		0x35
#define LCD_MEMCONTROL			0x36
#define LCD_VSCROLLSTARTADDRESS	0x37
#define LCD_IDLEMODEOFF			0x38
#define LCD_IDLEMODEON			0x39
#define LCD_PIXELFORMAT			0x3A
#define LCD_WRITEMEMCONTINUE	0x3C
#define LCD_READMEMCONTINUE		0x3E
#define LCD_SETSCANLINE			0x44
#define LCD_GETSCANLINE			0x45
#define LCD_WRITEBRIGHTNESS		0x51
#define LCD_READBRIGHTNESS		0x52
#define LCD_WRITECTRL			0x53
#define LCD_READCTRL			0x54
#define LCD_WRITECABC			0x55
#define LCD_READCABC			0x56
#define LCD_WRITECABCMIN		0x5E
#define LCD_READCABCMIN			0x5F
#define LCD_RGBSIGNALCONTROL	0xB0
#define LCD_FRAMECONTROLNORMAL	0xB1
#define LCD_FRAMECONTROLIDLE	0xB2
#define LCD_FRAMECONTROLPARTIAL	0xB3
#define LCD_INVERSIONCONTROL	0xB4
#define LCD_BLANKINGPORCHCONT	0xB5
#define LCD_DISPLAYFUNC			0xB6
#define LCD_ENTRYMODE			0xB7
#define LCD_BACKLIGHTCONTROL1	0xB8
#define LCD_BACKLIGHTCONTROL2	0xB9
#define LCD_BACKLIGHTCONTROL3	0xBA
#define LCD_BACKLIGHTCONTROL4	0xBB
#define LCD_BACKLIGHTCONTROL5	0xBC
#define LCD_BACKLIGHTCONTROL7	0xBE
#define LCD_BACKLIGHTCONTROL8	0xBF
#define LCD_POWERCONTROL1		0xC0
#define LCD_POWERCONTROL2		0xC1
#define LCD_VCOMCONTROL1		0xC5
#define LCD_VCOMCONTROL2		0xC7
#define LCD_NVMEMORYWRITE		0xD0
#define LCD_NVMEMORYKEY			0xD1
#define LCD_NVMEMORYSTATUSREAD	0xD2
#define LCD_READID4				0xD3
#define LCD_READID1				0xDA
#define LCD_READID2				0xDB
#define LCD_READID3				0xDC
#define LCD_POSITIVEGAMMCORR	0xE0
#define LCD_NEGATIVEGAMMCORR	0xE1
#define LCD_DIGITALGAMMCONTROL1	0xE2
#define LCD_DIGITALGAMMCONTROL2	0xE3
#define LCD_INTERFACECONTROL	0xF6
// LCD Registers

void lcd_init(void);
void lcd_fill_RGB(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void lcd_set_orientation(uint8_t Orientation);
void lcd_set_window(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
void lcd_display_off(void);
void lcd_display_on(void);
void lcd_data_write(unsigned short data);
void lcd_cmd_write(unsigned char command);
void lcd_backlight_on();
void lcd_backlight_off();

lv_coord_t lcd_get_width();
lv_coord_t lcd_get_height();
uint8_t lcd_get_orientation();



#endif /* __LCD_FSMC_H_ */
