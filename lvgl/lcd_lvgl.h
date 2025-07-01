#ifndef __LCD_LVGL_H
#define __LCD_LVGL_H

#include <lcd_fsmc.h>
#include <lvgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "XPT2046_touch.h"


void lvgl_init();
void lvgl_xpt2046_touch_init();

#endif /*__LCD_LVGL_H */
