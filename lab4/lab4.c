// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>
#include "mouse_test.h"
#include "macros.h"
#include "i8254.h"

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
  struct packet pp;


  message msg;
  uint8_t mouse_id;
  uint32_t r;

  if(mouse_enable() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }
  //mouse_enable_data_reporting();

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

          /////// Parte a rever ///////////
          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }

          if(byteNumber == 3){
            counter++;
            print_packet(&pp);
            byteNumber = 0;
          }
        }
        ////// Até aqui ////////////

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
    return 1;
  }

  OB_cleaner(); // Clear the output buffer
  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
 struct packet pp;
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
  OB_cleaner();

  while (counter < cnt){
    //printf("Counter: %x\n", counter);
    if (byteNumber == 0){
      write_kbc(READ_DATA);
    }

    mouse_ih();

    if (kbc_ih_error == true){
      byteNumber = 0;
      continue;
    }

    if(byteNumber == 3){
      print_packet(&pp);
      byteNumber = 0;
      counter++;
      tickdelay (micros_to_ticks(period*1000));
    }

  }

  if(mouse_disable() != OK){
    return 1;
  }

  if(set_stream_mode() != OK){
    return 1;
  }

  if(enable_mouse_interrupts()!= OK){
    return 1;
  }
  OB_cleaner();

  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_async)(uint8_t idle_time) {

  uint8_t bit_no_mouse, bit_no_timer;
  uint32_t erro;
  struct packet pp;

  if (timer_subscribe_int(&bit_no_timer)) { //subscribes the timer
    printf("Error in timer_subscribe_int", 0);
    return 1;
  }

 if(mouse_enable() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }
  //mouse_enable_data_reporting();

  if(mouse_subscribe(&bit_no_mouse) != 0){
    printf("Error subscribing mouse notifications\n");
    return 1;
  }

  uint32_t irq_set_mouse = BIT(bit_no_mouse);
  uint32_t irq_set_timer = BIT(bit_no_timer);

  int ipc_status;
  message msg;

  while (counter_t / 60 < idle_time) { //Stops if ESC is pressed or has no input for n seconds
    /* Get a request message. */
    //printf("Counter %x\n", counter_t);

    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set_timer) {/* subscribed interrupt */
          timer_int_handler();
        }
        if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }

          if(byteNumber == 3){
            print_packet(&pp);
            byteNumber = 0;
            counter_t = 0;
          }
        }

        break;

        default:
        break; /* no other notifications expected: do nothing */
      }

    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if ( timer_unsubscribe_int()) { //unsibscribes the timer
    printf("Error in timer_unsbscribe_int", 0);
    return 1;
  }
  if(mouse_unsubscribe() != 0){
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if(mouse_disable() != 0){
    printf("Error disabling mouse data reporting\n");
    return 1;
  }

  OB_cleaner();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance){

  int ipc_status;
  message msg;
  uint8_t mouse_id;
  uint32_t r;
  struct packet pp;
  struct mouse_ev event;
  event.delta_x = 0;
  event.delta_y = 0;


  /*if(mouse_enable() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }
*/
  mouse_enable_data_reporting();

  if(mouse_subscribe(&mouse_id) != 0){
    printf("Error subscribing mouse notifications\n");
    return -1;
  }
  uint32_t irq_set = BIT(mouse_id);

  OB_cleaner(); // Clear the output buffer

  while(state != COMP) {

    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

          mouse_ih();

          if (kbc_ih_error == true){
            byteNumber = 0;
            continue;
          }
            if(byteNumber == 3){
              print_packet(&pp);
              byteNumber = 0;
                printf("ola %x\n", state);
              
              if (!pp.rb && pp.lb && !pp.mb){ //if left button is pressed
                if (state == INIT || state == VERTEX){
                  event.type = LB_PRESSED;
                gesture_handler(&event, x_len); continue;}
                printf("%x\n", state);
                event.delta_x += pp.delta_x;
                event.delta_y += pp.delta_y;
                if (pp.delta_x > -tolerance && pp.delta_y > -tolerance){ //displacements in x and y
                  if (event.delta_y / event.delta_x > 0){ //positive slope
                    event.type = MOUSE_MOV;
                    printf("%x\n", state);
                    gesture_handler(&event, x_len);
                  }
                  else{
                    event.type = BUTTON_EV;
                    gesture_handler(&event, x_len);
                    printf("%x\n", state);
                  }
                }else{
                  event.type = BUTTON_EV;
                  gesture_handler(&event, x_len);
                   printf("%x\n", state);
                }
              }else if (pp.rb && !pp.lb && !pp.mb){ //if rigth button is pressed
                if(state == VERTEX || state == VERTEX){
                  event.type = RB_PRESSED;
                gesture_handler(&event, x_len);
                continue;
              }
              printf("%x\n", state);
                event.delta_x += pp.delta_x;
                event.delta_y += pp.delta_y;
                if (pp.delta_x > tolerance && pp.delta_y > tolerance){ //displacements in x and y
                  if (event.delta_y / event.delta_x < 0){ //positive slope
                    event.type = MOUSE_MOV;
                    gesture_handler(&event, x_len);
                     printf("%x\n", state);
                  }
                  else{
                    event.type = BUTTON_EV;
                    gesture_handler(&event, x_len);
                     printf("%x\n", state);
                  }
                }else{
                  event.type = BUTTON_EV;
                  gesture_handler(&event, x_len);
                   printf("%x\n", state);
                }

              }else if (!pp.rb && !pp.lb && !pp.mb){ //if no button is pressed
                if (state == LINE1 || state == DRAW1){
                  event.type = LB_RELEASED;
                }else if (state == LINE2 || state == DRAW2){
                  event.type = RB_RELEASED;
                }
                else if(state == VERTEX) {
                  if (abs(pp.delta_x) <= tolerance && abs(pp.delta_y) <= tolerance){
                    event.type = MOUSE_MOV;
                  }
                  else{
                    event.type = BUTTON_EV;
                  }

                }
                else
                event.type = BUTTON_EV;

                gesture_handler(&event, x_len);
                printf("%x\n", state);
              }


              else{ //if the middle button is pressed or more than one button is pressed
                event.type = BUTTON_EV;
                gesture_handler(&event, x_len);
                printf("%x\n", state);
              }




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
    return 1;
  }

  OB_cleaner(); // Clear the output buffer
  return 0;
}
