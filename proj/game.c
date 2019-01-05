#include <lcom/lcf.h>
#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "keyboard.h"
#include "mouse.h"
#include "video_gr.h"
#include "i8254.h"
#include "i8042.h"
#include "initGame.h"
#include "spriteMovement.h"
#include "pointSystem.h"
#include "serial_port.h"

void singlePlayerGame(){
  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t timer_irq_set = getTIMER_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();
  uint32_t rtc_irq_set = getRTC_IRQ();

  uint8_t nbyte = 0; //scancode's number of bytes
  bool wait = false;

  loadGameNumbers(); //Creates Bitmaps for all numbers

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
  resetBallSpeed();
  throwBall(ball);

  while (scanByte != ESC_CODE) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & rtc_irq_set) {
          update_date();
        }
        if (msg.m_notify.interrupts & kbc_irq_set) {
          kbc_asm_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);

          if (!wait) {
            set_move(player, nbyte, '0',0);
          }
          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();

          if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
            throwBall(ball);
            if(!pointHandler(aim, (int)getHorResolution()/2,1010)){
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
          printPoints(0,0,0);
          print_date();
          doubleBuffCall();

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
        break;
      }
    } else {
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

  uint8_t nbyte = 0; //scancode's number of bytes
  bool wait = false;

  uint16_t movToSend = 0x0000;
  uint16_t tempX = 0x0000;
  uint16_t tempY = 0x0000;
  uint8_t toSend = 0x00;
  uint8_t charSentNumber=0x00;
  bool isTransmiting = false;

  unsigned int SMSpeedX;
  unsigned int SMSpeedY;

  int player1Score = 0;
  int player2Score = 0;

  uint32_t receivedMov = 0;
  uint8_t charRecNumber = 0;

  loadGameNumbers(); //Creates Bitmaps for all numbers

  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* multiPlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/MultiPlayerField.bmp");
  Bitmap* player1Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1RightHand.bmp");
  Bitmap* player1Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1LeftHand.bmp");
  Bitmap* player2Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2RightHand.bmp");
  Bitmap* player2Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2LeftHand.bmp");
  Bitmap* win = loadBitmap("/home/lcom/labs/proj/bmp/YouWin.bmp");
  Bitmap* lose = loadBitmap("/home/lcom/labs/proj/bmp/YouLose.bmp");


  Sprite* ball = createSprite(ball_bmp,200,500,0,0);
  Sprite* player1 = createSprite(player1Right_bmp, 20,getVerResolution()/2,0,0);
  Sprite* player2 = createSprite(player2Right_bmp, getHorResolution()- player2Right_bmp->bitmapInfoHeader.width ,getVerResolution()/2,0,0);
  Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
  drawBitmap(multiPlayerField_bmp,0,0, ALIGN_LEFT);
  drawSprite(player1);
  drawSprite(player2);
  drawSprite(ball);
  drawSprite(aim);
  doubleBuffCall();

  update_date();
  clean_RBR();

  while (scanByte != ESC_CODE) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & rtc_irq_set) {
          update_date();
        }

        if (msg.m_notify.interrupts & kbc_irq_set) {
          kbc_asm_ih();
          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);
          if (!wait ) {
            set_move(player1, nbyte,'0',1);
          }

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();


          if(player1Score == 5){
            drawBitmap(win, 244,236, ALIGN_LEFT);
            doubleBuffCall();
            sleep(2);
            scanByte = ESC_CODE;
            continue;
          } else if (player2Score == 5){
            drawBitmap(lose, 244,236, ALIGN_LEFT);
            doubleBuffCall();
            sleep(2);
            scanByte = ESC_CODE;
            continue;
          }

          if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
            if(!pointHandler(aim, (int)getHorResolution(),1010)){
              player1Score++;
              ball->xspeed = 0;
              ball->yspeed = 0;
              ball->x = 90;
              ball->y = 500;
            }else {
              player2Score++;
              ball->xspeed = 0;
              ball->yspeed = 0;
              ball->x = 940;
              ball->y = 310;
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


          if(charSentNumber == 0){
            if(ball->colided) {
              if (is_left_pressed()){
                shootBall(ball, aim);
                isTransmiting = true;

                SMSpeedX = ((ball->xspeed + (ball->xspeed >> 31)) ^ (ball->xspeed >> 31)) | ((ball->xspeed >> 31) << 4); //converts so signal and magnitude
                SMSpeedY = ((ball->yspeed + (ball->yspeed >> 31)) ^ (ball->yspeed >> 31)) | ((ball->yspeed >> 31) << 4);

                movToSend = 0x0000 |(BIT(15)| BIT(14) | BIT(13) | BIT(12)| BIT(11)| BIT(10)); //prepara a head e a tail


                tempY = (SMSpeedY & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 5;
                tempX = (SMSpeedX & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 0;

                movToSend = movToSend |tempY | tempX; // int preparado para enviar
                toSend = (movToSend & 0xff00) >> 8;
                write_THR(toSend);
                charSentNumber++;
              }
            }
          } else if (charSentNumber == 1){
            toSend = (movToSend & 0x00ff) >> 0;
            write_THR(toSend);
            isTransmiting = false;
            charSentNumber = 0;
          }

          drawBitmap(multiPlayerField_bmp,0, 0, ALIGN_LEFT);
          drawSprite(player1);
          drawSprite(player2);
          drawSprite(aim);
          drawSprite(ball);
          printPoints(1, player1Score, player2Score);
          print_date();
          doubleBuffCall();
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

          if(((charReceived & 0xF8) == 0xF8) && (charRecNumber == 0)){
            printf("charRecNumber: %d\n", charRecNumber);
            receivedMov = 0x0000 |(charReceived << 8);
            charRecNumber++;
          } else if(charRecNumber == 1){
            printf("charRecNumber: %d\n", charRecNumber);
            receivedMov = receivedMov |(charReceived << 0);
            charRecNumber= 0;
            printTest(receivedMov);
            remoteMoveBall(ball, receivedMov);
          } else{
            printf("Not a ball packet\n" );
            set_move(player2, 1, charReceived,1);
            charRecNumber = 0;
          }
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
  deleteBitmap(multiPlayerField_bmp);
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

  uint8_t nbyte = 0; //scancode's number of bytes
  bool wait = false;

  uint16_t movToSend = 0x0000;
  uint16_t tempX = 0x0000;
  uint16_t tempY = 0x0000;
  uint8_t toSend = 0x00;
  uint8_t charSentNumber=0x00;
  bool isTransmiting = false;

  unsigned int SMSpeedX;
  unsigned int SMSpeedY;

  int player1Score = 0;
  int player2Score = 0;

  uint16_t receivedMov = 0;
  uint8_t charRecNumber = 0;

  loadGameNumbers(); //Creates Bitmaps for all numbers

  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* multiPlayerField_bmp = loadBitmap("/home/lcom/labs/proj/bmp/MultiPlayerField.bmp");
  Bitmap* player2Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2RightHand.bmp");
  Bitmap* player2Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player2LeftHand.bmp");
  Bitmap* player1Right_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1RightHand.bmp");
  Bitmap* player1Left_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Player1LeftHand.bmp");
  Bitmap* win = loadBitmap("/home/lcom/labs/proj/bmp/YouWin.bmp");
  Bitmap* lose = loadBitmap("/home/lcom/labs/proj/bmp/YouLose.bmp");

  Sprite* ball = createSprite(ball_bmp,200,500,0,0);
  Sprite* player2 = createSprite(player2Right_bmp, getHorResolution()- player2Right_bmp->bitmapInfoHeader.width ,getVerResolution()/2,0,0);
  Sprite* player1 = createSprite(player1Right_bmp, 20,getVerResolution()/2,0,0);
  Sprite* aim = createSprite(aim_bmp, getHorResolution()/2,getVerResolution()/2,0,0);
  drawBitmap(multiPlayerField_bmp,0,0, ALIGN_LEFT);
  drawSprite(player2);
  drawSprite(player1);
  drawSprite(ball);
  drawSprite(aim);
  doubleBuffCall();

  update_date();
  clean_RBR();

  while (scanByte != ESC_CODE) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & rtc_irq_set) {
          update_date();
        }
        if (msg.m_notify.interrupts & kbc_irq_set) {
          kbc_asm_ih();
          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);
          if (!wait ) {
            set_move(player2, nbyte,'0',1);
          }

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set) {
          timer_int_handler();

          if(player1Score == 5){
            drawBitmap(lose, 244,236, ALIGN_LEFT);
            doubleBuffCall();
            sleep(2);
            scanByte = ESC_CODE;
            continue;
          } else if (player2Score == 5){
            drawBitmap(win, 244,236,ALIGN_LEFT);
            doubleBuffCall();
            sleep(2);
            scanByte = ESC_CODE;
            continue;
          }

          if(ball->y <= 70 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
            if(!pointHandler(aim, 13,(int) getHorResolution()/2)){
              player1Score++;
              ball->xspeed = 0;
              ball->yspeed = 0;
              ball->x = 940;
              ball->y = 310;
            }else
            player2Score++;
            ball->xspeed = 0;
            ball->yspeed = 0;
            ball->x = 90;
            ball->y = 500;
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

          if(charSentNumber == 0){
            if(ball->colided) {
              if (is_left_pressed()){
                shootBall(ball, aim);
                isTransmiting = true;

                SMSpeedX = ((ball->xspeed + (ball->xspeed >> 31)) ^ (ball->xspeed >> 31)) | ((ball->xspeed >> 31) << 4); //converts so signal and magnitude
                SMSpeedY = ((ball->yspeed + (ball->yspeed >> 31)) ^ (ball->yspeed >> 31)) | ((ball->yspeed >> 31) << 4);

                movToSend = 0x0000 |(BIT(15)| BIT(14) | BIT(13) | BIT(12)| BIT(11)| BIT(10)); //prepara a head e a tail


                tempY = (SMSpeedY & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 5;
                tempX = (SMSpeedX & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 0;

                movToSend = movToSend |tempY | tempX; // int preparado para enviar
                toSend = (movToSend & 0xff00) >> 8;
                write_THR(toSend);
                charSentNumber++;
              }
            }
          } else if (charSentNumber == 1){
            toSend = (movToSend & 0x00ff) >> 0;
            write_THR(toSend);
            isTransmiting = false;
            charSentNumber = 0;
          }

          /*if(ball->colided || isTransmiting) {
          if (is_left_pressed() && !isTransmiting){
          shootBall(ball, aim);
          isTransmiting = true;
        }
        if(charSentNumber == 0){

        SMSpeedX = ((ball->xspeed + (ball->xspeed >> 31)) ^ (ball->xspeed >> 31)) | ((ball->xspeed >> 31) << 4); //converts so signal and magnitude
        SMSpeedY = ((ball->yspeed + (ball->yspeed >> 31)) ^ (ball->yspeed >> 31)) | ((ball->yspeed >> 31) << 4);

        movToSend = 0x0000 |(BIT(15)| BIT(14) | BIT(13) | BIT(12)| BIT(11)| BIT(10)); //prepara a head e a tail


        tempY = (SMSpeedY & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 5;
        tempX = (SMSpeedX & (BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))) << 0;

        movToSend = movToSend |tempY | tempX; // int preparado para enviar
        toSend = (movToSend & 0xff00) >> 8;
        write_THR(toSend);
        charSentNumber++;
      } else if (charSentNumber == 1){
      toSend = (movToSend & 0x00ff) >> 0;
      write_THR(toSend);
      isTransmiting = false;
      charSentNumber = 0;
    }
  }*/

  drawBitmap(multiPlayerField_bmp,0, 0, ALIGN_LEFT);
  drawSprite(player2);
  drawSprite(player1);
  drawSprite(aim);
  drawSprite(ball);
  printPoints(1,player1Score,player2Score);
  print_date();
  doubleBuffCall();
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
  //set_move(player1, 1, charReceived,1);

  if(((charReceived & 0xF8) == 0xF8) && (charRecNumber == 0)){
    printf("charRecNumber: %d\n", charRecNumber);
    receivedMov = 0x0000 |(charReceived << 8);
    charRecNumber++;
  } else if(charRecNumber == 1){
    printf("charRecNumber: %d\n", charRecNumber);
    receivedMov = receivedMov |(charReceived << 0);
    printTest(receivedMov);
    charRecNumber = 0;
    remoteMoveBall(ball, receivedMov);
  } else{
    printf("Not a ball packet\n" );
    set_move(player1, 1, charReceived,1);
    charRecNumber = 0;
  }
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
deleteBitmap(multiPlayerField_bmp);
deleteBitmap(player2Right_bmp);
deleteBitmap(player2Left_bmp);
deleteBitmap(player1Right_bmp);
deleteBitmap(player1Left_bmp);
}
