// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "mous.h"
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


  message msg;
  uint8_t mouse_id;
  uint32_t r;


  if(mouse_enable() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }
  if(mouse_subscribe(&mouse_id) != 0){
  printf("Error subscribing mouse notifications\n");
  return -1;
  }
   uint32_t irq_set = BIT(mouse_id);

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
                    mouse_handler();

                  if (byteNumber == 4){
                    printf("Error in mouse handler\n",0 );
                  }

                  if(byteNumber == 2){
                    counter++;
                    print_packet();
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


if(mouse_unsubscribe() != 0){
      printf("The program was unable to unsubscribe a mouse notification\n");
      return 1;
    }

    if(mouse_disable() != 0){
  printf("Error disabling mouse data reporting\n");
  return -1;
}
OB_cleaner(); // Clear the output buffer
return 0;
}


int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  
uint32_t counter = 0;

    if(disable_mouse_interrupts() != OK){
      return 1;
    }

    if(mouse_disable() != OK){
      return 1;
    }

    if(set_remote_mode() != OK){
      return 1;
    }

    write_kbc(READ_DATA);
    while (counter < cnt){
      mouse_handler();
      if(byteNumber == 4 || byteNumber == 0){
        write_kbc(READ_DATA);}
      if(byteNumber == 2){
        print_packet();
        tickdelay (micros_to_ticks(period*1000));
        counter++;
      }
    }


    if(set_stream_mode() != OK){
      return 1;
    }

    if(mouse_enable() != OK){
      return 1;
    }

    if(enable_mouse_interrupts()!= OK){
      return 1;
    }

    return 0;
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
