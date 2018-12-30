#include <lcom/lcf.h>
#include "playerMovement.h"
#include "i8042.h"
#include "bitmap.h"
#include "keyboard.h"
#include "video_gr.h"
#include "serial_port.h"
#include <math.h>

int playerMovSpeed = 7;
int ballMovSpeed = 10;
static int aimx = 0;
static int aimy = 0;

//charReceived deve ser '0' quando é para mexer o player controlado pelo user
// ou então é  w a s d i j k l consoante o char recebido no serial port
void set_move(Sprite *sp, uint8_t nbyte, char adversaryMov) {
  if(adversaryMov == '0'){
    if((nbyte == 1 && scanByte==KEY_W) || (nbyte == 2 && scanByte == KEY_UP )) {
      sp->mov.MOVE_UP = true;
      write_THR('w');
    }else if((nbyte == 1 && scanByte==KEY_S) || (nbyte == 2 && scanByte == KEY_DOWN)) {
      sp->mov.MOVE_DOWN = true;
      write_THR('s');
    }else if((nbyte == 1 && scanByte==KEY_A) || (nbyte == 2 && scanByte == KEY_LEFT)) {
      sp->mov.MOVE_LEFT = true;
      write_THR('a');
    }else if((nbyte == 1 && scanByte==KEY_D) || (nbyte == 2 && scanByte == KEY_RIGTH)) {
      sp->mov.MOVE_RIGHT = true;
      write_THR('d');
    }else if((nbyte == 1 && scanByte==KEY_W_BREAK) || (nbyte == 2 && scanByte == KEY_UP_BREAK)) {
      sp->mov.MOVE_UP = false;
      write_THR('i');
    }else if((nbyte == 1 && scanByte==KEY_S_BREAK) || (nbyte == 2 && scanByte == KEY_DOWN_BREAK)) {
      sp->mov.MOVE_DOWN = false;
      write_THR('k');
    }else if((nbyte == 1 && scanByte==KEY_A_BREAK) || (nbyte == 2 && scanByte == KEY_LEFT_BREAK)) {
      sp->mov.MOVE_LEFT = false;
      write_THR('j');
    }else if((nbyte == 1 && scanByte==KEY_D_BREAK) || (nbyte == 2 && scanByte == KEY_RIGTH_BREAK)) {
      sp->mov.MOVE_RIGHT = false;
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
  /*  printf("x %d\n",ballx );
  printf("y %d\n",bally );
  printf("x %d\n",aimx );
  printf("y %d\n",aimy );*/

  double angle = atan2((double)(aimy - bally),(aimx - ballx));
  float l = angle;
  printf("a %f \n",l);
  char buffer[20];
  sprintf (buffer,  "a %f", l );
  printf("%s\n", buffer);

  //  printf("t %d\n", ((double)(aimy - bally)/(aimx - ballx)));

  ball->xspeed = ballMovSpeed * cos(angle);
  ball->yspeed = ballMovSpeed * sin(angle);
  //  printf("x %d\n",ball->xspeed );
  //  printf("y %d\n",ball->yspeed );
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
  int angle = ((rand() % 60) - 30);
  ball->x = 4* getHorResolution()/5;
  ball->y = getVerResolution()/2;
  printf("reset\n");
  ball->xspeed = -ballMovSpeed * cos(angle*M_PI/180);
  ball->yspeed = -ballMovSpeed * sin(angle*M_PI/180);
  ball->xspeed = -7;
  ball->yspeed = 0;
}
