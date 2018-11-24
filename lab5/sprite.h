#ifndef _SPRITE_H_
#define _SPRITE_H_

typedef struct {
  int x, y;            // current position
  int width, height;   // dimensions
  int xspeed, yspeed;  // current speed
  char *map;           // the pixmap
} Sprite;

//int drawSprite (const char *pic[], uint16_t x, uint16_t y);
Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);
//void draw_sprite(Sprite *sp);
//void erase_sprite(Sprite *sp);
void destroy_sprite(Sprite *sp);

#endif
