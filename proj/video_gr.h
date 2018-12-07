#ifndef _VIDEO_GR_H_
#define _VIDEO_GR_H_

#include <stdint.h>

int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t* vmi_p);
int col(uint16_t x, uint16_t y, uint32_t color);
int drawPattern (uint8_t no_rectangles, uint32_t first, uint8_t step);
//int drawSprite (const char *xpm[], uint16_t x, uint16_t y);
//void draw_sprite(Sprite *sp);
//void erase_sprite(Sprite *sp);
int vbe_get_controller_info();

//void move_sprite(Sprite *sprite, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed);

void doubleBuffCall();

int getHorResolution();
int getVerResolution();
char *getGraphicsBuffer();

#endif
