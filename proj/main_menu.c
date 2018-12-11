#include <lcom/lcf.h>
#include <lcom/timer.h>
// Any header files included below this line should have been created by you
#include "i8254.h"
#include "i8042.h"
#include "bitmap.h"
#include "keyboard.h"
#include "main_menu.h"
#include "video_gr.h"
#include "vbe_macros.h"
#include "game.h"

typedef enum{SINGLE_PL, MULTI_PL, HIGHSCORE, EXIT}menu_options;

static menu_options opt = SINGLE_PL;
static uint32_t timer_irq_set;
static uint32_t kbc_irq_set;
uint32_t getKBC_IRQ(){
  return kbc_irq_set;
}
uint32_t getTIMER_IRQ(){
  return timer_irq_set;
}
int start_main_menu (){
  uint8_t timer_bit_no,kbc_bit_no;
  int ipc_status;
  message msg;
  uint32_t r;
  printf("ola\n");

  if(keyboard_subscribe(&kbc_bit_no) != OK){
    printf("Error enabling keyboard interrupts",0);
    return 1;
  }
  if(timer_subscribe_int(&timer_bit_no) != OK){
    printf("Error enabling timer interrupts",0);
    return 1;
  }
  kbc_irq_set = BIT(kbc_bit_no);
  timer_irq_set = BIT(timer_bit_no);

  if(vg_init(0x117) == NULL){
    printf("Error setting graphics mode\n");
    return 1;
  }

  bool exits = 0;

  Bitmap* menu_back = loadBitmap("/home/lcom/labs/proj/bmp/MenuBackground.bmp");
  Bitmap* selec = loadBitmap("/home/lcom/labs/proj/bmp/Selector.bmp");
  drawBitmap(menu_back,0, 0, ALIGN_LEFT);
  Sprite* selector = createSprite(selec, 47,264,0,0);
  drawSprite(selector);
  doubleBuffCall();

  while (!exits) {
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
            if(scanByte==KEY_S){
              opt =((opt + 1) % (EXIT+1));
              printf("%x option\n", opt);
            }
            if(scanByte==KEY_W){
              opt = ((opt - 1) % (EXIT+1));
              printf("%x option\n", opt);
            }
            if(scanByte==KEY_ENTER){
              call_func_menu();
              if (opt == EXIT || opt == HIGHSCORE){
                exits = 1;
              }
            }
            switch(opt){
              case SINGLE_PL:
              selector->y=264;
              break;
              case MULTI_PL:
              selector->y=383;
              break;
              case HIGHSCORE:
              selector->y=501;
              break;
              case EXIT:
              selector->y=618;
              break;
            }
            drawBitmap(menu_back,0, 0, ALIGN_LEFT);
            drawSprite(selector);
            doubleBuffCall();

            tickdelay(micros_to_ticks(90000));
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

  if (timer_unsubscribe_int() != 0) {
    printf("Error disabling timer interrupts\n");
    return 1;
  }

  if (keyboard_unsubscribe() != 0) {
    printf("Error disabling keyboard interrupts\n");
    return 1;
  }

  vg_exit();
  printf("Set to text mode\n");
  return 0;

}


void call_func_menu(){

  if (opt == SINGLE_PL){
    initGame();
    opt = SINGLE_PL;
    return;

  }else if(opt == MULTI_PL){
    initGame();
    opt = SINGLE_PL;
    return;

  }else if(opt == EXIT){
    return;
  }else if (opt == HIGHSCORE){
    return;
  }

}
