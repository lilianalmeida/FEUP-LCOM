#ifndef _VIDEO_GR_H_
#define _VIDEO_GR_H_

int (vg_initialize)(uint16_t mode);
int (col)(uint16_t x, uint16_t y, uint32_t color);
int (drawSquare)(uint16_t x,uint16_t y,uint16_t width, uint16_t height, uint32_t color);

#endif
