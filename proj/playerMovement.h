#ifndef _PLAYERMOV_H_
#define _PLAYERMOV_H_
#include "bitmap.h"


void set_move(Sprite *sp, uint8_t nbyte);
void movePlayer(Sprite* sp);
void throwBall(Sprite* ball);
void shootBall(Sprite* ball, Sprite* aim);
int getAimx();
int getAimy();
void resetAim();

#endif
