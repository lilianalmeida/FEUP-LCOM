#ifndef _PLAYERMOV_H_
#define _PLAYERMOV_H_
#include "bitmap.h"


void set_move(Sprite *sp, uint8_t nbyte, char adversaryMov, bool isMulti);
void movePlayer(Sprite* sp, uint32_t xLeft, uint32_t xRight);
void throwBall(Sprite* ball);
void shootBall(Sprite* ball, Sprite* aim);
int getAimx();
int getAimy();
void resetAim();

#endif
