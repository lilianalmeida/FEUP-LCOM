#ifndef _GAME_H_
#define _GAME_H_

void movePlayer(Sprite* sp);
void set_move(Sprite *sp, uint8_t nbyte);
void throwBall(Sprite* ball);
void initGame();
void shootBall(Sprite* ball, Sprite* aim);

#endif
