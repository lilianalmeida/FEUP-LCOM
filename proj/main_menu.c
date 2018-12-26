#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"
#include "bitmap.h"
#include "keyboard.h"
#include "initGame.h"
#include "mouse_test.h"
#include "main_menu.h"
#include "video_gr.h"
#include "game.h"
#include "pointSystem.h"

static menu_options opt = SINGLE_PL;

static bool exits = 0;

void start_main_menu (){

  int ipc_status;

  message msg;

  uint32_t r;

  uint8_t nbyte = 0; //numero de bytes do scancode

  bool wait = false;

  uint32_t kbc_irq_set = getKBC_IRQ();

  uint32_t mouse_irq_set = getMOUSE_IRQ();

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

          isTwoByte(&wait, &nbyte);

          if (wait == false) {
            scancode_parse(scanByte, nbyte);
          }

          parse_kbc_keys(selector, menu_back, nbyte);

          tickdelay(micros_to_ticks(DELAY_US));

        }

        if (msg.m_notify.interrupts & mouse_irq_set) {

          OB_cleaner();

        }
        break;
        default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }
  deleteBitmap(menu_back);
  deleteBitmap(selec);
  
}

void parse_kbc_keys(Sprite *selector, Bitmap *menu_back, uint8_t nbyte) {
  if ((nbyte == 1 && scanByte==KEY_S) || (nbyte == 2 && scanByte == KEY_DOWN)) {
    opt = ((opt + 1) % (EXIT + 1));
  }

  else if ((nbyte == 1 && scanByte==KEY_W) || (nbyte == 2 && scanByte == KEY_UP)) {
    opt = ((opt - 1) % (EXIT + 1));
  }

  else if (scanByte == KEY_ENTER) {
    call_func_menu();
  }

  switch (opt) {
    case SINGLE_PL:
    selector->y = 264;
    break;

    case MULTI_PL:
    selector->y = 383;
    break;

    case HIGHSCORE:
    selector->y = 501;
    break;

    case EXIT:
    selector->y = 618;
    break;
  }

  drawBitmap(menu_back, 0, 0, ALIGN_LEFT);

  drawSprite (selector);

  doubleBuffCall();

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

    exits = 1;

    return;

  }else if (opt == HIGHSCORE){

    highscoreScreen();

    opt = SINGLE_PL;

    return;

  }

}
