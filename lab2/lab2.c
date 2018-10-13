#include <lcom/lcf.h>

#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"

//global variable
uint32_t counter = 0;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


//// 7.1 ////
int (timer_test_read_config)(uint8_t timer, enum timer_status_field field) {

  if (timer > 2 || timer < 0){
    printf("Timer not found. Choose a timer between 0 and 2",0);
    return 1;
  }

  uint8_t st;

  int erro = timer_get_conf(timer, &st);
  if (erro != OK) {
    printf("Error in timer_get_conf", 0);
    return erro;
  }

  erro = timer_display_conf(timer, st, field);
  if (erro != OK) {
    printf("Error in timer_display_conf", 0);
    return erro;
  }

  return 0;
}


//// 7.2 ////
int (timer_test_time_base)(uint8_t timer, uint32_t freq) {

  if (timer > 2 || timer < 0){
    printf("Timer not found",0);
    return 1;
  }

  int erro = timer_set_frequency(timer, freq);
  if (erro != OK) {
    printf("Error in timer_set_frequency", 0);
    return erro;
  }

  return 0;
}


//// 7.3 ////
int (timer_test_int)(uint8_t time) {

  uint8_t bit_no = 1;

  int erro=timer_subscribe_int(&bit_no);
  if (erro != OK) {
    printf("Error in timer_subscribe_int", 0);
    return erro;
  }

  int ipc_status;
  message msg;
  uint32_t irq_set = bit_no;

  while (counter / 60 < time) {
    /* Get a request message. */
    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          /* process it */
          timer_int_handler();
          if (counter % 60 == 0) {
            timer_print_elapsed_time();
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

  erro=timer_unsubscribe_int();
  if (erro != OK) {
    printf("Error in timer_unsbscribe_int", 0);
    return erro;
  }

  return 0;
}


//// 7.2 ////
int (util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = (uint8_t) val;

  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {

  val = val >> 8;
  *msb = (uint8_t) val;

  return 0;
}
