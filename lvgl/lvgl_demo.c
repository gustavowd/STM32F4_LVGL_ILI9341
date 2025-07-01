/*
 * lvgl_demo.c
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */

#include <lcd_fsmc.h>
#include "lvgl.h"
#include <stdio.h>
#include <stdlib.h>

static lv_obj_t * arc;
extern float sht40_temp, sht40_numi;

static void arc_value_changed_event_cb(lv_event_t * e)
{
    lv_obj_t * arc = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);

    lv_label_set_text_fmt(label, "%d%%", lv_arc_get_value(arc));
    lv_arc_align_obj_to_angle(arc, label, 25);

    //lv_arc_rotate_obj_to_angle(arc, label, 25);
}

void mbtn_cb(lv_event_t* e) {

	lv_arc_set_value(arc, lv_arc_get_value(arc)-1);
	lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL);
}

void pbtn_cb(lv_event_t* e) {

	lv_arc_set_value(arc, lv_arc_get_value(arc)+1);
	lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL);
}

void rbtn_cb(lv_event_t* e) {
	uint8_t rot = lcd_get_orientation();
	lcd_set_orientation((rot+1)%4);
	lv_disp_t* disp = lv_disp_get_default();
	disp->driver->hor_res = lcd_get_width();
	disp->driver->ver_res = lcd_get_height();

	lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);

}

void demo_screen_rotation() {
	lv_obj_t * label = lv_label_create(lv_scr_act());

	arc = lv_arc_create(lv_scr_act());
	lv_obj_set_size(arc, 170, 170);
	lv_arc_set_rotation(arc, 150);
	lv_arc_set_bg_angles(arc, 0, 240);
	lv_arc_set_mode(arc, LV_ARC_MODE_SYMMETRICAL);
	lv_arc_set_value(arc, 50);
	lv_obj_align(arc,LV_ALIGN_TOP_MID, 0, 25);
	lv_obj_add_event_cb(arc, arc_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);


	lv_event_send(arc, LV_EVENT_VALUE_CHANGED, NULL);

	lv_obj_t *pbtn = lv_btn_create(lv_scr_act());
	lv_obj_t *plabel = lv_label_create(pbtn);
	lv_label_set_text(plabel, LV_SYMBOL_PLUS);
	lv_obj_align(pbtn, LV_ALIGN_BOTTOM_MID, 30, -20);
	lv_obj_set_size(pbtn, 40, 30);
	lv_obj_add_event_cb(pbtn, pbtn_cb, LV_EVENT_CLICKED, NULL);

	lv_obj_t *mbtn = lv_btn_create(lv_scr_act());
	lv_obj_t *mlabel = lv_label_create(mbtn);
	lv_label_set_text(mlabel, LV_SYMBOL_MINUS);
	lv_obj_align(mbtn, LV_ALIGN_BOTTOM_MID, -30, -20);
	lv_obj_set_size(mbtn, 40, 30);
	lv_obj_add_event_cb(mbtn, mbtn_cb, LV_EVENT_CLICKED, NULL);

	lv_obj_t *rbtn = lv_btn_create(lv_scr_act());
	lv_obj_t *rlabel = lv_label_create(rbtn);
	lv_label_set_text(rlabel, "R");
	lv_obj_align(rbtn, LV_ALIGN_BOTTOM_RIGHT, -10, -20);
	lv_obj_set_size(rbtn, 40, 30);
	lv_obj_add_event_cb(rbtn, rbtn_cb, LV_EVENT_CLICKED, NULL);

}

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
void dma_demo() {
	uint16_t color[240*32];
	uint16_t tc[3] = {0xf100, 0x7e0, 0x1f};
	uint32_t t1;
	uint16_t width=lcd_get_width();
	uint16_t height=lcd_get_height();
	int  sum=0;

	lv_obj_t* label = lv_label_create(lv_scr_act());
	lv_obj_center(label);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);
	lv_label_set_text(label, "With DMA \n    vs\n without DMA");
	lv_timer_handler();
	HAL_Delay(3000);
	lv_label_set_text(label, "Start with DMA(preload):\n10 frame\n500ms delay \nbetween frames");
	lv_timer_handler();
	HAL_Delay(3000);

	for (int i = 0; i < 10; i++) {
		//t1 = HAL_GetTick();
		for (int j = 0; j < 240*32; j++) color[j] = tc[i%3];
		t1 = HAL_GetTick();
			lcd_set_window(0, 0, width-1, height-1);

			// use memory-to-memory DMA
			for (int k=0; k < 10; k++) {
				//for (int j = 0; j < 240*32; j++) color[j] = tc[i%3];
				HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t)color, (LCD_DATA_BASE), 240*32);
				HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 1000);
				}
		sum += HAL_GetTick()-t1;
		HAL_Delay(500);
	}
	lv_label_set_text_fmt(label, "End with DMA:\n10 frames \nTotal Time:%d ms", sum);
	lv_timer_handler();

	HAL_Delay(5000);
	lv_label_set_text(label, "Start without DMA:\n10 frame\n500ms delay \nbetween frames");
	lv_timer_handler();
	HAL_Delay(3000);
	sum=0;
	for (int i = 0; i < 10; i++) {

		t1 = HAL_GetTick();

		lcd_fill_RGB(tc[i%3], 0, 0, width, height);

		sum += HAL_GetTick()-t1;
		HAL_Delay(500);
	}
	lv_label_set_text_fmt(label, "End without DMA:\n10 frames \nTotal Time:%d ms", sum);
	lv_timer_handler();
	HAL_Delay(5000);
}

uint8_t test_with_dma=true;
void sd_img_dma_demo() {
	uint32_t t1;
	int sum=0;
	char fn[20];
	lv_obj_t* img = lv_img_create(lv_scr_act());
	lv_obj_center(img);
	lv_obj_t* label = lv_label_create(lv_scr_act());
	lv_obj_center(label);
	lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);
	lv_label_set_text(label, "With DMA \n       vs\n without DMA");
	lv_timer_handler();
	HAL_Delay(3000);
	if (test_with_dma)
		lv_label_set_text(label, "Start with DMA:\n10 frame\n500ms delay \nbetween frames");
	else
		lv_label_set_text(label, "Start without DMA:\n10 frame\n500ms delay \nbetween frames");
	lv_timer_handler();
	HAL_Delay(3000);
	lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
	sum=0;

	for (int i =0; i < 10; i++) {
		t1 = HAL_GetTick();
		sprintf(fn, "S:img%d.bmp", i);
		lv_img_set_src(img, fn);
		lv_timer_handler();
		sum += (HAL_GetTick()-t1);
		HAL_Delay(500);
	}

	lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
	if (test_with_dma)
		lv_label_set_text_fmt(label, "End with DMA:\n10 frames \nTotal Time:%d ms", sum);
	else
		lv_label_set_text_fmt(label, "End without DMA:\n10 frames \nTotal Time:%d ms", sum);
	lv_timer_handler();
	HAL_Delay(5000);


}

