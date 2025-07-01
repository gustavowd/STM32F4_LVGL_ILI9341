/*
 * lcd_lvgl.c
 *
 *  Created on: Jun 30, 2025
 *      Author: gustavo
 */

#include <lcd_lvgl.h>

/* Memory-to-memory DMA Handler */
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

extern uint8_t test_with_dma;
void tft_lvgl_draw_bitmap(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *bitmap)
{
	uint32_t total_pixels = (x2-x1+1) * (y2-y1+1);

	lcd_set_window(x1, y1, x2, y2);

	// use memory-to-memory DMA
	HAL_DMA_Start(&hdma_memtomem_dma2_stream0, (uint32_t)bitmap, (LCD_DATA_BASE), total_pixels);
	HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream0, HAL_DMA_FULL_TRANSFER, 1000);

	// if not use DMA translation
	//	for (int i=0; i < total_pixels; i++) {
	//		lcd_data_write(*(bitmap+i));
	//	}

}

void tft_lvgl_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{
    tft_lvgl_draw_bitmap(
            (uint16_t)(area->x1),
            (uint16_t)(area->y1),
            (uint16_t)(area->x2),
            (uint16_t)(area->y2), (uint16_t*)color_p
            );

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}


void lvgl_init() {
	lv_init();

	static lv_disp_draw_buf_t draw_buf;
	static lv_color_t buf1[LCD_WIDTH * LCD_HEIGHT / 10];                        /*Declare a buffer for 1/10 screen size*/
	lv_disp_draw_buf_init(&draw_buf, buf1, NULL, LCD_WIDTH * LCD_HEIGHT / 10);  /*Initialize the display buffer.*/

	static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
	disp_drv.flush_cb = tft_lvgl_disp_flush;    /*Set your driver function*/
	disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
	disp_drv.hor_res = lcd_get_width();   /*Set the horizontal resolution of the display*/
	disp_drv.ver_res = lcd_get_height();   /*Set the vertical resolution of the display*/
	lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

}

void lvgl_xpt2046_read_cb(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
	static uint16_t x, y;
	if(XPT2046_TouchPressed()) {
		XPT2046_TouchGetCoordinates(&x, &y);
		data->point.x = x;
		data->point.y = y;
		data->state = LV_INDEV_STATE_PRESSED;
	} else {
		data->state = LV_INDEV_STATE_RELEASED;
	}
}

void lvgl_xpt2046_touch_init() {
	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);      	/*Basic initialization*/
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = lvgl_xpt2046_read_cb;
	/*Register the driver in LVGL and save the created input device object*/
	lv_indev_drv_register(&indev_drv);
}

