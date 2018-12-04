// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>
// Any header files included below this line should have been created by you
#include "i8254.h"
#include "i8042.h"
#include "bitmap.h"
#include "keyboard.h"
#include "mouse_test.h"
#include "mouse_macros.h"
#include "video_gr.h"
#include "vbe_macros.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
  return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) {

  uint8_t timer_bit_no,kbc_bit_no;
  int ipc_status;
  message msg;
  uint32_t r;

  if(keyboard_subscribe(&kbc_bit_no) != OK){
    printf("Error enabling keyboard interrupts",0);
    return 1;
  }
  if(timer_subscribe_int(&timer_bit_no) != OK){
    printf("Error enabling timer interrupts",0);
    return 1;
  }
  uint32_t kbc_irq_set = BIT(kbc_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);

  if(vg_init(0x117) == NULL){
    printf("Error setting graphics mode\n");
    return 1;
  }

  Bitmap* bmp = loadBitmap("/home/lcom/labs/proj/bmp/Minix.bmp");
  Bitmap* field = loadBitmap("/home/lcom/labs/proj/bmp/field.bmp");

  int y = 20,x=20;
  drawBitmap(field,0, 0, ALIGN_LEFT);
  drawBitmap(bmp,x, y, ALIGN_LEFT);
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
          if(counter_t % 60 == 0){

            if(scanByte==0x0011){
              y-= 5;
              drawBitmap(field,0, 0, ALIGN_LEFT);
              drawBitmap(bmp,x, y, ALIGN_LEFT);
            }
            if(scanByte==0x001F){
              y+= 5;
              x+=5;
              drawBitmap(field,0, 0, ALIGN_LEFT);
              drawBitmap(bmp,x, y, ALIGN_LEFT);
            }
            if(scanByte==0x001E){
              x-= 5;
              if(x < 0){
                x+=5;
                drawBitmap(field,0, 0, ALIGN_LEFT);
                drawBitmap(bmp,x, y, ALIGN_LEFT);
              }
              drawBitmap(field,0, 0, ALIGN_LEFT);
              drawBitmap(bmp,x, y, ALIGN_LEFT);
            }
            if(scanByte==0x0020){
              x+= 5;
              if(x + bmp->bitmapInfoHeader.width > getHorResolution()/2){
                x-=5;
                drawBitmap(field,0, 0, ALIGN_LEFT);
                drawBitmap(bmp,x, y, ALIGN_RIGHT);
              }
              drawBitmap(field,0, 0, ALIGN_LEFT);
              drawBitmap(bmp,x, y, ALIGN_LEFT);
            }
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

  if (keyboard_unsubscribe() != 0) {
    printf("Error disabling keyboard interrupts\n");
    return 1;
  }

  vg_exit();
  printf("Set to text mode\n");
  return 0;
}
