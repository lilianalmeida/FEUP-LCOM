#ifndef _VIDEO_GR_H_
#define _VIDEO_GR_H_

//int (vg_initialize)(uint16_t mode);
int (col)(uint16_t x, uint16_t y, uint32_t color);
//int (drawRect)(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint32_t color);
int (drawPattern) (uint8_t no_rectangles, uint32_t first, uint8_t step);
//int vg_draw_hline	(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (drawSprite) (const char *xpm[], uint16_t x, uint16_t y);

#endif
