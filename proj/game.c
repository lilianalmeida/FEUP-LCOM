#include <lcom/lcf.h>
#include "bitmap.h"
#include "keyboard.h"
#include "mouse_test.h"
#include "mouse_macros.h"
#include "video_gr.h"
#include "vbe_macros.h"
#include "i8254.h"
#include "i8042.h"
#include "main_menu.h"



void initGame(){
  int ipc_status;
  message msg;
  uint32_t r;
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t timer_irq_set = getTIMER_IRQ();

  Bitmap* ball_bmp = loadBitmap("/home/lcom/labs/proj/bmp/Bola.bmp");
  Bitmap* aim = loadBitmap("/home/lcom/labs/proj/bmp/Crosshair.bmp");
  Bitmap* field = loadBitmap("/home/lcom/labs/proj/bmp/Field.bmp");
  Sprite* bola = createSprite(ball_bmp, 20,20,0,0);
  Sprite* sp = createSprite(aim, 20,20,0,0);
  drawBitmap(field,0,0, ALIGN_LEFT);
  drawSprite(sp);
  drawSprite(bola);
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

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & timer_irq_set){
          timer_int_handler();
          if(counter_t % 2 == 0){
            if(counter_t % 60 == 0){
              bola->x = 0;
              bola->y = 0;
              printf("reset\n");
            }
            bola->x++;
            bola->y++;
            if(scanByte==KEY_W){
              sp->y -=5;
            }
            if(scanByte==KEY_S){
              sp->y +=5;
            }
            if(scanByte==KEY_A){
              sp->x -=5;
              if(sp->x < 3 ){
                sp->x +=5;
              }
            }
            if(scanByte==KEY_D){
              sp->x +=5;
              if(sp->x + sp->width > (uint32_t)getHorResolution()/2){
                sp->x -=5;

              }

            }

            drawBitmap(field,0, 0, ALIGN_LEFT);
            drawSprite(sp);
            drawSprite(bola);
            doubleBuffCall();
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
