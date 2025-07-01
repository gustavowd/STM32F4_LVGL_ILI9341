/*
 * lvgl_demo.h
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */

#ifndef LVGL_DEMO_H_
#define LVGL_DEMO_H_

void demo_screen_rotation();
void dma_demo();
void sd_img_dma_demo();

extern lv_obj_t* temp_bar;
extern uint8_t test_with_dma;

#endif /* LVGL_DEMO_H_ */
