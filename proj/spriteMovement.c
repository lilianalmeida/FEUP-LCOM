#include <lcom/lcf.h>
#include <math.h>

#include "spriteMovement.h"
#include "i8042.h"
#include "keyboard.h"
#include "video_gr.h"
#include "serial_port.h"

int playerMovSpeed = 7; //speed of the player's movement
double ballMovSpeed = 9; //initial ball speed
static int aimx = 0; //position x of the aim of the mouse when ball collides with the player's racket
static int aimy = 0; //position y of the aim of the mouse when ball collides with the player's racket

//charReceived is '0' when the movement belongs to the player controlled by the user
// or it's 'w' 'a' 's' 'd' 'i' 'j' 'k' 'l' depending on the char received by the serial port
void set_move(Sprite *sp, uint8_t nbyte, char adversaryMov, bool isMulti) {
  if(adversaryMov == '0'){
    if((nbyte == 1 && scanByte==KEY_W) || (nbyte == 2 && scanByte == KEY_UP )) {
      sp->mov.MOVE_UP = true;
      if(isMulti)
      write_THR('w');
    }else if((nbyte == 1 && scanByte==KEY_S) || (nbyte == 2 && scanByte == KEY_DOWN)) {
      sp->mov.MOVE_DOWN = true;
      if(isMulti)
      write_THR('s');
    }else if((nbyte == 1 && scanByte==KEY_A) || (nbyte == 2 && scanByte == KEY_LEFT)) {
      sp->mov.MOVE_LEFT = true;
      if(isMulti)
      write_THR('a');
    }else if((nbyte == 1 && scanByte==KEY_D) || (nbyte == 2 && scanByte == KEY_RIGHT)) {
      sp->mov.MOVE_RIGHT = true;
      if(isMulti)
      write_THR('d');
    }else if((nbyte == 1 && scanByte==KEY_W_BREAK) || (nbyte == 2 && scanByte == KEY_UP_BREAK)) {
      sp->mov.MOVE_UP = false;
      if(isMulti)
      write_THR('i');
    }else if((nbyte == 1 && scanByte==KEY_S_BREAK) || (nbyte == 2 && scanByte == KEY_DOWN_BREAK)) {
      sp->mov.MOVE_DOWN = false;
      if(isMulti)
      write_THR('k');
    }else if((nbyte == 1 && scanByte==KEY_A_BREAK) || (nbyte == 2 && scanByte == KEY_LEFT_BREAK)) {
      sp->mov.MOVE_LEFT = false;
      if(isMulti)
      write_THR('j');
    }else if((nbyte == 1 && scanByte==KEY_D_BREAK) || (nbyte == 2 && scanByte == KEY_RIGHT_BREAK)) {
      sp->mov.MOVE_RIGHT = false;
      if(isMulti)
      write_THR('l');
    }
  } else if( adversaryMov != '0') {

    if(adversaryMov == 'w'){
      sp->mov.MOVE_UP = true;

    }else if(adversaryMov == 's') {
      sp->mov.MOVE_DOWN = true;

    }else if(adversaryMov == 'a') {
      sp->mov.MOVE_LEFT = true;

    }else if(adversaryMov == 'd') {
      sp->mov.MOVE_RIGHT = true;

    }else if(adversaryMov == 'i') {
      sp->mov.MOVE_UP = false;

    }else if(adversaryMov == 'k') {
      sp->mov.MOVE_DOWN = false;

    }else if(adversaryMov == 'j') {
      sp->mov.MOVE_LEFT = false;

    }else if(adversaryMov == 'l') {
      sp->mov.MOVE_RIGHT = false;
    }
  }


}

void movePlayer(Sprite* sp, uint32_t xLeft, uint32_t xRight){

  if(sp->mov.MOVE_UP){
    sp->y -=playerMovSpeed;
    if (sp-> y <= 70){
      sp->y = 71;
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
    if(sp->x <(int) xLeft){
      sp->x = xLeft;
    }
  }

  if(sp->mov.MOVE_RIGHT){
    sp->x +=playerMovSpeed;
    if(sp->x + sp->width >= (unsigned int )xRight){
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

  double angle = atan2((double)(aimy - bally),(aimx - ballx));
  float l = angle;
  printf("a %f \n",l);
  char buffer[20];
  sprintf (buffer,  "a %f", l );
  printf("%s\n", buffer);

  ball->xspeed = ballMovSpeed * cos(angle);
  ball->yspeed = ballMovSpeed * sin(angle);

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
  int angle = ((rand() % 50) - 25);
  ball->x = 4* getHorResolution()/5;
  ball->y = getVerResolution()/2;
  printf("reset\n");
  ball->xspeed = -ballMovSpeed* cos(angle*M_PI/180);
  ball->yspeed = -ballMovSpeed * sin(angle*M_PI/180);

  ballMovSpeed += 0.1;//increases difficulty
}

void resetBallSpeed(){
  ballMovSpeed = 9;
}

