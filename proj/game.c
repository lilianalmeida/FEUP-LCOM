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
#include "playerMovement.h"
#include "pointSystem.h"

void initGame() {
  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t timer_irq_set = getTIMER_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();

  loadNumbers(); //Creates sprites for all numbers

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

  Bitmap* ballThrower_bmp = loadBitmap("/home/lcom/labs/proj/bmp/BallThrower.bmp");
  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* singlePlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/SinglePlayerField.bmp");
  Bitmap* playerRight_bmp = loadBitmap("/home/lcom/labs/proj/bmp/PlayerRightHand.bmp");
  Bitmap* playerLeft_bmp = loadBitmap("/home/lcom/labs/proj/bmp/PlayerLeftHand.bmp");


  Sprite* ball = createSprite(ball_bmp,4*getHorResolution()/5,getVerResolution()/2,0,0);
  Sprite* player = createSprite(playerRight_bmp, 20,getVerResolution()/2,0,0);
  Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
  Sprite* ballThrower = createSprite(ballThrower_bmp,4*getHorResolution()/5,getVerResolution()/2-30,0,0);
  drawBitmap(singlePlayerField_bmp,0,0, ALIGN_LEFT);
  drawSprite(player);
  drawSprite(ball);
  drawSprite(aim);
  doubleBuffCall();

  throwBall(ball);
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
          isTwoByte(&wait, &nbyte);
          if (wait == false) {
            scancode_parse(scanByte, nbyte);
          }
          set_move(player, nbyte);

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();


          if(counter_t % 1 == 0) {

            if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
              throwBall(ball);
              if(!pointHandler(ball, aim)){
                scanByte = ESC_CODE;
              }
            }
            if(ball->y < player->y + (int)player->height/2){ //Código para mudar o sprite do player consoante a posicao da bola
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

            drawBitmap(singlePlayerField_bmp,0, 0, ALIGN_LEFT);
            drawSprite(ballThrower);
            drawSprite(player);
            drawSprite(aim);
            drawSprite(ball);
            printPoints();
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
