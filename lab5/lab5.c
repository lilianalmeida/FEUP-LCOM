// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "vbe_test.h"
#include "macro.h"
#include "video_gr.h"
#include "keyboard.h"


extern uint32_t scanByte;
bool kbc_ih_error = false;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
  return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {

  vg_initialize(mode);
  tickdelay (micros_to_ticks (delay*1000000));
  vg_exit();


  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  uint8_t bit_no;
  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

  int erro = keyboard_subscribe(&bit_no);
  if (erro != OK) {
    printf("Error in keyboard_subscribe", 0);
    return erro;
  }
  uint32_t irq_set = BIT(bit_no);
  int ipc_status;
  message msg;



  if(vg_initialize(mode) != 0){
    printf("Error setting graphics mode\n");
    return 1;
  }

  drawSquare(x,y,width, height,color);
  //video_dump_fb();

  while (scanByte != ESC_CODE) {
    /* Get a request message. */
    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

          kbc_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
          } else {

            isTwoByte(&wait, &nbyte);
            if (wait == false) {
              erro = scancode_parse(scanByte, nbyte);
              if (erro != OK)
              return erro;
            }

          }

          tickdelay (micros_to_ticks(DELAY_US));


          break;
        }
          default:
          break; /* no other notifications expected: do nothing */
        }
      }
      else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
    }

    erro = keyboard_unsubscribe();
    if (erro != OK) {
      printf("Error in keyboard_unsbscribe", 0);
      return erro;
    }

    vg_exit();
    printf("Set to text mode\n");
    return 0;

  }

  /*int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first) {

  printf("%s(0x%03x, %u, 0x%08x): under construction\n", __func__, mode, no_rectangles, first);

  return 1;
}*/
