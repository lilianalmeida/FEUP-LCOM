#include <lcom/lcf.h>
#include <stdlib.h>

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

  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* field = loadBitmap("/home/lcom/labs/proj/bmp/Field.bmp");
  Sprite* ball = createSprite(ball_bmp, 0,0,0,0);
  Sprite* sp = createSprite(aim, 20,20,0,0);
  Sprite* aimm = createSprite(aim, 150,150,0,0);
  drawBitmap(field,0,0, ALIGN_LEFT);
  drawSprite(sp);
  drawSprite(ball);
  drawSprite(aimm);
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
			
			set_move(sp);

			tickdelay(micros_to_ticks(DELAY_US));
		}
		if (msg.m_notify.interrupts & timer_irq_set) {
			timer_int_handler();
			if(counter_t % 1 == 0) {

				if(ball->y < 1 || ball->y > getVerResolution()|| ball->x < 1 || ball->x > getHorResolution()) {
					throwBall(ball);
				}

				ball->x += ball->xspeed;
				ball->y += ball->yspeed;

				movePlayer(sp);
				if(ball->colided) {
					if (is_left_pressed()){
						ball->colided = false;
						ball->canColide = false;
						ball->x += abs(ball->xspeed);
						ball->xspeed = -ball->xspeed;
						ball->yspeed = -ball->yspeed;
					}
				}

				drawBitmap(field,0, 0, ALIGN_LEFT);
				drawSprite(sp);
				drawSprite(aimm);
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
            move_aim (aimm);
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

void throwBall(Sprite* ball) {
  ball->x = 3* getHorResolution()/4;
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
