#include <lcom/lcf.h>
#include "i8042.h"
#include "bitmap.h"
#include "keyboard.h"
#include "video_gr.h"
#include <math.h>

int playerMovSpeed = 7;
int ballMovSpeed = 6;
static int aimx = 0;
static int aimy = 0;


void set_move(Sprite *sp, uint8_t nbyte) {

  if((nbyte == 1 && scanByte==KEY_W) || (nbyte == 2 && scanByte == KEY_UP)) {
    sp->mov.MOVE_UP = true;

  }else if((nbyte == 1 && scanByte==KEY_S) || (nbyte == 2 && scanByte == KEY_DOWN)) {
    sp->mov.MOVE_DOWN = true;

  }else if((nbyte == 1 && scanByte==KEY_A) || (nbyte == 2 && scanByte == KEY_LEFT)) {
    sp->mov.MOVE_LEFT = true;

  }else if((nbyte == 1 && scanByte==KEY_D) || (nbyte == 2 && scanByte == KEY_RIGTH)) {
    sp->mov.MOVE_RIGHT = true;

  }else if((nbyte == 1 && scanByte==KEY_W_BREAK) || (nbyte == 2 && scanByte == KEY_UP_BREAK)) {
    sp->mov.MOVE_UP = false;

  }else if((nbyte == 1 && scanByte==KEY_S_BREAK) || (nbyte == 2 && scanByte == KEY_DOWN_BREAK)) {
    sp->mov.MOVE_DOWN = false;

  }else if((nbyte == 1 && scanByte==KEY_A_BREAK) || (nbyte == 2 && scanByte == KEY_LEFT_BREAK)) {
    sp->mov.MOVE_LEFT = false;

  }else if((nbyte == 1 && scanByte==KEY_D_BREAK) || (nbyte == 2 && scanByte == KEY_RIGTH_BREAK)) {
    sp->mov.MOVE_RIGHT = false;
  }

}

void movePlayer(Sprite* sp){

  if(sp->mov.MOVE_UP){
    sp->y -=playerMovSpeed;
    if (sp-> y < 0){
      sp->y = 0;
    }
  }

  if(sp->mov.MOVE_DOWN){
    sp->y +=playerMovSpeed;
    if ((sp->y + sp->height)>= (uint32_t) getVerResolution()){
      sp->y -=playerMovSpeed;
    }
  }

  if(sp->mov.MOVE_LEFT){
    sp->x -=playerMovSpeed;
    if(sp->x < 0){
      sp->x = 0;
    }
  }

  if(sp->mov.MOVE_RIGHT){
    sp->x +=playerMovSpeed;
    if(sp->x + sp->width >= (uint32_t)getHorResolution()/2){
      sp->x -=playerMovSpeed;
    }
  }
}
void shootBall(Sprite* ball, Sprite* aim){
  ball->colided = false;
  ball->canColide = false;

  int ballx = ball->x;
  int bally = ball->y;
  aimx = aim->x;
  aimy = aim->y;
  printf("x %d\n",ballx );
  printf("y %d\n",bally );
  printf("x %d\n",aimx );
  printf("y %d\n",aimy );

  double angulo = atan2((double)(aimy - bally),(double)(aimx - ballx));
  printf("a %d\n",angulo );
  printf("t %d\n", ((double)(aimy - bally)/(double)(aimx - ballx)));

  ball->xspeed = ballMovSpeed * cos(angulo);
  ball->yspeed = ballMovSpeed * sin(angulo);
  printf("x %d\n",ball->xspeed );
  printf("y %d\n",ball->yspeed );
}

int getAimx(){
  return aimx;
}

int getAimy(){
  return aimy;
}
void resetAim(){
  aimx = 0;
  aimy =0;
}

void throwBall(Sprite* ball) {
  ball->x = 4* getHorResolution()/5;
  ball->y = getVerResolution()/2;
  printf("reset\n");
  ball->xspeed = -((rand() % 3) + ballMovSpeed);
  ball->yspeed = ((rand() % (ballMovSpeed*2)) - ballMovSpeed);
  ball->xspeed = -5;
  ball->yspeed = 0;
}
