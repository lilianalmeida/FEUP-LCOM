// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse.h"
#include "macros.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  uint32_t counter = 0;
  uint32_t byte_array[3];
  message msg;
  int mouse_id = 12;
  uint32_t irq_set = BIT(mouse_id);
  uint32_t r;

  if(mouse_subscribe(&mouse_id) != 0){
  printf("Error subscribing mouse notifications\n");
  return -1;
  }

  if(mouse_enable() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

OB_cleaner(); // Clear the output buffer

while(counter < cnt) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d\n", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                  // Calls the handler, and if it returns 0, then byte_array is ready for printing
                  if(mouse_handler(byte_array) == 0){
                    counter++;
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

if(mouse_disable() != 0){
  printf("Error disabling mouse data reporting\n");
  return -1;
}

if(mouse_unsubscribe(&mouse_id) != 0){
      printf("The program was unable to unsubscribe a mouse notification\n");
      return 1;
    }
OB_cleaner(); // Clear the output buffer
return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)() {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}
