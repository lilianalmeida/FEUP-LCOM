#include <lcom/lcf.h>
#include <stdlib.h>
#include <math.h>

#include "bitmap.h"
#include "keyboard.h"
#include "mouse_test.h"
#include "mouse_macros.h"
#include "video_gr.h"
#include "i8254.h"
#include "i8042.h"
#include "initGame.h"
#include "game.h"

int playerMovSpeed = 7;

void initGame() {
  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t timer_irq_set = getTIMER_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();

  Bitmap* ballThrower_bmp = loadBitmap("/home/lcom/labs/proj/bmp/BallThrower.bmp");
  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* field_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Field.bmp");
  Bitmap* playerRight_bmp = loadBitmap("/home/lcom/labs/proj/bmp/PlayerRightHand.bmp");
  Bitmap* playerLeft_bmp = loadBitmap("/home/lcom/labs/proj/bmp/PlayerLeftHand.bmp");


  Sprite* ball = createSprite(ball_bmp, 0,0,0,0);
  Sprite* player = createSprite(playerRight_bmp, 20,20,0,0);
  Sprite* aim = createSprite(aim_bmp, 150,150,0,0);
  Sprite* ballThrower = createSprite(ballThrower_bmp,4*getHorResolution()/5,getVerResolution()/2-30,0,0);
  drawBitmap(field_bmp,0,0, ALIGN_LEFT);
  drawSprite(player);
  drawSprite(ball);
  drawSprite(aim);
  doubleBuffCall();

  while (scanByte != ESC_CODE) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */

          kbc_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }

          set_move(player);

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();
          if(counter_t % 1 == 0) {

            if(ball->y < 1 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
              throwBall(ball);
            }
            if(ball->y < player->y){ //Código para mudar o sprite do player consoante a posicao da bola
              player->bmp = playerLeft_bmp;
            }else {
              player->bmp = playerRight_bmp;
            }

            ball->x += ball->xspeed;
            ball->y += ball->yspeed;

            movePlayer(player);
            if(ball->colided) {
              if (is_left_pressed()){
                shootBall(ball, aim);
              }
            }

            drawBitmap(field_bmp,0, 0, ALIGN_LEFT);
            drawSprite(ballThrower);
            drawSprite(player);
            drawSprite(aim);
            drawSprite(ball);
            doubleBuffCall();

          }

        }
        if (msg.m_notify.interrupts & mouse_irq_set) {
          mouse_ih();

          if (mouse_ih_error == true){
            byteNumber = 0;
            continue;
          }

          if(byteNumber == 3){
            counter++;
            parse_packet();
            move_aim (aim);
            byteNumber = 0;
          }
        }
        break;
        default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

}
void shootBall(Sprite* ball, Sprite* aim){
  ball->colided = false;
  ball->canColide = false;
  ball->x += abs(ball->xspeed);

  int ballx = ball->x;
  int bally = ball->y;
  int aimx = aim->x;
  int aimy = aim->y;
  printf("x %d\n",ballx );
  printf("y %d\n",bally );
  printf("x %d\n",aimx );
  printf("y %d\n",aimy );

  double angulo = atan2((double)(aimy - bally),(double)(aimx - ballx));
  printf("a %d\n",angulo );
  printf("t %d\n", ((double)(aimy - bally)/(double)(aimx - ballx)));

  ball->xspeed = 4 * cos(angulo);
  ball->yspeed = 4 * sin(angulo);
  printf("x %d\n",ball->xspeed );
  printf("y %d\n",ball->yspeed );
}

void throwBall(Sprite* ball) {
  ball->x = 4* getHorResolution()/5;
  ball->y = getVerResolution()/2;
  printf("reset\n");
  ball->xspeed = -((rand() % 3) + 5);
  ball->yspeed = (rand() % 6) -3;
}

void set_move(Sprite *sp) {

  if(scanByte==KEY_W) {
    sp->mov.MOVE_UP = true;

  }else if(scanByte==KEY_S) {
    sp->mov.MOVE_DOWN = true;

  }else if(scanByte==KEY_A) {
    sp->mov.MOVE_LEFT = true;

  }else if(scanByte==KEY_D) {
    sp->mov.MOVE_RIGHT = true;

  }else if(scanByte==KEY_W_BREAK) {
    sp->mov.MOVE_UP = false;

  }else if(scanByte==KEY_S_BREAK) {
    sp->mov.MOVE_DOWN = false;

  }else if(scanByte==KEY_A_BREAK) {
    sp->mov.MOVE_LEFT = false;

  }else if(scanByte==KEY_D_BREAK) {
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
