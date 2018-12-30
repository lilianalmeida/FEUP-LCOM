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
#include "rtc_macros.h"
#include "serial_port.h"

void singlePlayerGame() {
  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t timer_irq_set = getTIMER_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();
  uint32_t rtc_irq_set = getRTC_IRQ();

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

  loadGameNumbers(); //Creates Bitmaps for all numbers

  printf("1\n" );

  Bitmap* ballThrower_bmp = loadBitmap("/home/lcom/labs/proj/bmp/BallThrower.bmp");
  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* singlePlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/SinglePlayerField.bmp");
  Bitmap* playerRight_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1RightHand.bmp");
  Bitmap* playerLeft_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1LeftHand.bmp");

  Sprite* ball = createSprite(ball_bmp,4*getHorResolution()/5,getVerResolution()/2,0,0);
  Sprite* player = createSprite(playerRight_bmp, 20,getVerResolution()/2,0,0);
  Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
  Sprite* ballThrower = createSprite(ballThrower_bmp,4*getHorResolution()/5,getVerResolution()/2-30,0,0);
  drawBitmap(singlePlayerField_bmp,0,0, ALIGN_LEFT);
  drawSprite(player);
  drawSprite(ball);
  drawSprite(aim);
  doubleBuffCall();



  update_date();
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
        if (msg.m_notify.interrupts & rtc_irq_set) {
          //TODO: Apagar ou nao?
          update_date();
        }
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
          set_move(player, nbyte, '0',0);
          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();

          if (counter_t % 60 == 0){
            update_date();
          }

          if(counter_t % 1 == 0) { //redundante
            if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
              throwBall(ball);
              if(!pointHandler(ball, aim)){
                scanByte = ESC_CODE;
              }
            }
            if(player->y < 412){ //Changes the racket hand
              player->bmp = playerLeft_bmp;
            }else {
              player->bmp = playerRight_bmp;
            }
            ball->x += ball->xspeed;
            ball->y += ball->yspeed;
            movePlayer(player,0, (uint32_t)getHorResolution()/2);
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
            print_date();
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
  deleteNumbers();
  deleteBitmap(ballThrower_bmp);
  deleteBitmap(ball_bmp);
  deleteBitmap(aim_bmp);
  deleteBitmap(singlePlayerField_bmp);
  deleteBitmap(playerRight_bmp);
  deleteBitmap(playerLeft_bmp);
}

void gamePlayer1(){
    if(waitingPlayer2() != 0){ //Assures both players are connected
      return ;
    }

    int ipc_status;
    message msg;
    uint32_t r;

    uint32_t kbc_irq_set = getKBC_IRQ();
    uint32_t timer_irq_set = getTIMER_IRQ();
    uint32_t mouse_irq_set = getMOUSE_IRQ();
    uint32_t rtc_irq_set = getRTC_IRQ();
    uint32_t uart_irq_set = getUART_IRQ();

    uint32_t charReceived = 'e'; //char random

    uint8_t nbyte = 0; //numero de bytes do scancode
    bool wait = false;

    loadGameNumbers(); //Creates Bitmaps for all numbers

    Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
    Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
    Bitmap* singlePlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/SinglePlayerField.bmp");
    Bitmap* player1Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1RightHand.bmp");
    Bitmap* player1Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1LeftHand.bmp");
    Bitmap* player2Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2RightHand.bmp");
    Bitmap* player2Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2LeftHand.bmp");

    Sprite* ball = createSprite(ball_bmp,90,500,0,0);
    Sprite* player1 = createSprite(player1Right_bmp, 20,getVerResolution()/2,0,0);
    Sprite* player2 = createSprite(player2Right_bmp, getHorResolution()- player2Right_bmp->bitmapInfoHeader.width ,getVerResolution()/2,0,0);
    Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
    drawBitmap(singlePlayerField_bmp,0,0, ALIGN_LEFT);
    drawSprite(player1);
    drawSprite(player2);
    drawSprite(ball);
    drawSprite(aim);
    doubleBuffCall();

    update_date();
    while (scanByte != ESC_CODE) {

      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d\n", r);
        continue;
      }
      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
          if (msg.m_notify.interrupts & rtc_irq_set) {
            //TODO: Apagar ou nao?
            update_date();
          }
          if (msg.m_notify.interrupts & kbc_irq_set) {
            kbc_ih();
            if (kbc_ih_error) {
              kbc_ih_error = false;
              continue;
            }
            isTwoByte(&wait, &nbyte);
            if (wait == false) {
              scancode_parse(scanByte, nbyte);
            }
            set_move(player1, nbyte,'0',1);

            tickdelay(micros_to_ticks(DELAY_US));
          }
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();

            if (counter_t % 60 == 0){
              update_date();
            }

            if(counter_t % 1 == 0) { //redundante
              if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
                if(!pointHandler(ball, aim)){
                  scanByte = ESC_CODE;
                }
              }
              if(player1->y < 412){ //Changes the racket hand
                player1->bmp = player1Left_bmp;
              }else {
                player1->bmp = player1Right_bmp;
              }
              if(player2->y < 412){ //Changes the racket hand
                player2->bmp = player2Left_bmp;
              }else {
                player2->bmp = player2Right_bmp;
              }
              ball->x += ball->xspeed;
              ball->y += ball->yspeed;
              movePlayer(player1, 0, (uint32_t) getHorResolution()/2);
              movePlayer(player2,(uint32_t) getHorResolution()/2,(uint32_t) getHorResolution());

              if(ball->colided) {
                if (is_left_pressed()){
                  shootBall(ball, aim);
                }
              }
              drawBitmap(singlePlayerField_bmp,0, 0, ALIGN_LEFT);
              drawSprite(player1);
              drawSprite(player2);
              drawSprite(aim);
              drawSprite(ball);
              printPoints();
              print_date();
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
          if(msg.m_notify.interrupts & uart_irq_set){
            serialPort_handler(&charReceived);
            set_move(player2, 1, charReceived,1);
          }
          break;
          default:
          break;
        }
      } else {

      }
    }
    deleteNumbers();
    deleteBitmap(ball_bmp);
    deleteBitmap(aim_bmp);
    deleteBitmap(singlePlayerField_bmp);
    deleteBitmap(player1Right_bmp);
    deleteBitmap(player1Left_bmp);
    deleteBitmap(player2Right_bmp);
    deleteBitmap(player2Left_bmp);

}
void gamePlayer2(){
    if(waitingPlayer1() != 0){//Assures both players are connected
      return ;
    }

    int ipc_status;
    message msg;
    uint32_t r;
    uint32_t kbc_irq_set = getKBC_IRQ();
    uint32_t timer_irq_set = getTIMER_IRQ();
    uint32_t mouse_irq_set = getMOUSE_IRQ();
    uint32_t rtc_irq_set = getRTC_IRQ();
    uint32_t uart_irq_set = getUART_IRQ();

    uint32_t charReceived = 'e'; //char random

    uint8_t nbyte = 0; //numero de bytes do scancode
    bool wait = false;

    loadGameNumbers(); //Creates Bitmaps for all numbers

    Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
    Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
    Bitmap* singlePlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/SinglePlayerField.bmp");
    Bitmap* player2Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2RightHand.bmp");
    Bitmap* player2Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2LeftHand.bmp");
    Bitmap* player1Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1RightHand.bmp");
    Bitmap* player1Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1LeftHand.bmp");

    Sprite* ball = createSprite(ball_bmp,90,500,0,0);
    Sprite* player2 = createSprite(player2Right_bmp, getHorResolution()- player2Right_bmp->bitmapInfoHeader.width ,getVerResolution()/2,0,0);
    Sprite* player1 = createSprite(player1Right_bmp, 20,getVerResolution()/2,0,0);
    Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
    drawBitmap(singlePlayerField_bmp,0,0, ALIGN_LEFT);
    drawSprite(player2);
    drawSprite(player1);
    drawSprite(ball);
    drawSprite(aim);
    doubleBuffCall();

    update_date();

    while (scanByte != ESC_CODE) {
      /* Get a request message. */
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d\n", r);
        continue;
      }
      if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & rtc_irq_set) {
            //TODO: Apagar ou nao?
            update_date();
          }
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
            set_move(player2, nbyte,'0',1);

            tickdelay(micros_to_ticks(DELAY_US));
          }
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();

            if (counter_t % 60 == 0){
              update_date();
            }

            if(counter_t % 1 == 0) { //redundante
              if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
                if(!pointHandler(ball, aim)){
                  scanByte = ESC_CODE;
                }
              }
              if(player2->y < 412){ //Changes the racket hand
                player2->bmp = player2Left_bmp;
              }else {
                player2->bmp = player2Right_bmp;
              }
              if(player1->y < 412){ //Changes the racket hand
                player1->bmp = player1Left_bmp;
              }else {
                player1->bmp = player1Right_bmp;
              }
              ball->x += ball->xspeed;
              ball->y += ball->yspeed;
              movePlayer(player2,(uint32_t) getHorResolution()/2,(uint32_t) getHorResolution());
              movePlayer(player1, 0, (uint32_t) getHorResolution()/2);
              if(ball->colided) {
                if (is_left_pressed()){
                  shootBall(ball, aim);
                }
              }
              drawBitmap(singlePlayerField_bmp,0, 0, ALIGN_LEFT);
              drawSprite(player2);
              drawSprite(player1);
              drawSprite(aim);
              drawSprite(ball);
              printPoints();
              print_date();
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
          if(msg.m_notify.interrupts & uart_irq_set){
            serialPort_handler(&charReceived);
            set_move(player1, 1, charReceived,1);
          }
          break;
          default:
          break; /* no other notifications expected: do nothing */
        }
      } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
    }
    deleteNumbers();
    deleteBitmap(ball_bmp);
    deleteBitmap(aim_bmp);
    deleteBitmap(singlePlayerField_bmp);
    deleteBitmap(player2Right_bmp);
    deleteBitmap(player2Left_bmp);
    deleteBitmap(player1Right_bmp);
    deleteBitmap(player1Left_bmp);
}
