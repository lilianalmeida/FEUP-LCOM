// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse_test.h"
#include "macros.h"
#include "i8254.h"

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

  if(cnt <=0){
    printf("Invalid parameter! Make sure cnt is greater than zero", 0);
    return 1;
  }

  int ipc_status;
  uint32_t counter = 0;

  message msg;
  uint8_t mouse_id;
  uint32_t r;

  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_enable_data() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

  if(mouse_subscribe(&mouse_id) != 0){
    printf("Error subscribing mouse notifications\n");
    return -1;
  }
  uint32_t irq_set = BIT(mouse_id);



  while(counter < cnt) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & irq_set) {
          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }

          if(byteNumber == 3){
            counter++;
            print_packet();
            byteNumber = 0;
          }
        }

        default:
        break;
      }
    } else {
    }
  }

  if(mouse_unsubscribe() != 0){
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if(mouse_disable_data() != 0){
    printf("Error disabling mouse data reporting\n");
    return 1;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {

  if(period <=0 || cnt <=0 || cnt > 255){
    printf("Invalid parameters! Make sure period and cnt are greater than zero and cnt is lower than 256.",0);
    return 1;
  }

  uint32_t counter = 0;

  while (counter < cnt){
    if (byteNumber == 0){
      write_kbc(READ_DATA);
    }

    mouse_ih();

    if (kbc_ih_error == true){
      byteNumber = 0;
      continue;
    }

    if(byteNumber == 3){
      print_packet();
      byteNumber = 0;
      counter++;
      tickdelay (micros_to_ticks(period*1000));
    }
  }

  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_disable_data() != OK){
    return 1;
  }

  if(enable_mouse_interrupts()!= OK){
    return 1;
  }
  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_async)(uint8_t idle_time) {

  if(idle_time <= 0){
    printf("Invalid parameter! Make sure idle_time is greater than zero.", 0);
    return 1;
  }

  uint8_t bit_no_mouse, bit_no_timer;
  uint32_t erro;


  if (timer_subscribe_int(&bit_no_timer)) {
    printf("Error in timer_subscribe_int", 0);
    return 1;
  }

  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_enable_data() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

  if(mouse_subscribe(&bit_no_mouse) != 0){
    printf("Error subscribing mouse notifications\n");
    return 1;
  }

  uint32_t irq_set_mouse = BIT(bit_no_mouse);
  uint32_t irq_set_timer = BIT(bit_no_timer);

  int ipc_status;
  message msg;

  while (counter_t / 60 < idle_time) {

    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_timer) {
          timer_int_handler();
        }
        if (msg.m_notify.interrupts & irq_set_mouse) {
          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }

          if(byteNumber == 3){
            print_packet();
            byteNumber = 0;
            counter_t = 0;
          }
        }

        break;

        default:
        break;
      }

    }
    else {
      /*do nothing */
    }
  }

  if ( timer_unsubscribe_int()) { //unsubscribes the timer
    printf("Error in timer_unsbscribe_int", 0);
    return 1;
  }
  if(mouse_unsubscribe() != 0){
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if(mouse_disable_data() != 0){
    printf("Error disabling mouse data reporting\n");
    return 1;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){

  if(x_len <= 0 || x_len > 255 || tolerance < 0 || tolerance > 255){
    printf("Invalid arguments! Make sure x_len is between 0 and 256 and tolerance is not negative.",0);
    return 1;
  }

  int ipc_status;
  message msg;
  uint8_t mouse_id;
  uint32_t r;

  struct mouse_ev event;
  event.delta_x = 0;
  event.delta_y = 0;

  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_enable_data() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

  if(mouse_subscribe(&mouse_id) != 0){
    printf("Error subscribing mouse notifications\n");
    return -1;
  }
  uint32_t irq_set = BIT(mouse_id);

  while(state != COMP) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & irq_set) {

          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }
          if(byteNumber == 3){
            print_packet();
            byteNumber = 0;

             mouse_events_handler(&event,x_len, tolerance);
           }
        }
        break;
        default:
        break;
      }
    } else {
      /* do nothing */
    }
  }

  if(mouse_unsubscribe() != 0){
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if(mouse_disable_data() != 0){
    printf("Error disabling mouse data reporting\n");
    return 1;
  }

  return 0;
}
