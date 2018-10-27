#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "macro.h"
#include "i8254.h"

extern uint32_t scanByte;
bool kbc_ih_error = false;
uint32_t scanByte_asm;
bool kbc_ih_error_asm;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (kbd_test_scan)(bool assembly) {

  if (assembly != 0 && assembly != 1) {
    printf("Assembly not valid in kbd_test_scan", 0);
    return 1;
  }

  uint8_t bit_no;

  int erro = keyboard_subscribe(&bit_no);
  if (erro != OK) {
    printf("Error in keyboard_subscribe", 0);
    return erro;
  }

  uint32_t irq_set = BIT(bit_no);
  int ipc_status;
  message msg;

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

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

          if (!assembly)
            kbc_ih();

          else if (assembly) {
            kbc_asm_ih();
            scanByte = scanByte_asm;
            kbc_ih_error = kbc_ih_error_asm;
          }

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

break;        }
        default:
        break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  if (!assembly) {
    erro = kbd_print_no_sysinb(counter);
    return erro;
  }

  erro = keyboard_unsubscribe();
  if (erro != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return erro;
  }

  counter = 0;
  return 0;
}

int (kbd_test_poll)() {

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;

  while (scanByte != ESC_CODE) {
    if (kbc_pol(scanByte) == -1) {
      continue;
    }

    isTwoByte(&wait, &nbyte);
    if (wait == false) {
      int erro = scancode_parse(scanByte, nbyte);
      if (erro != OK)
        return erro;
    }

    tickdelay (micros_to_ticks(DELAY_US));}

  if (kbd_print_no_sysinb (counter)) {
    return !OK;
  }

  if (interrupt_handler() != OK) {
    printf("Error in interrupt_handler", 0);
    return 1;
  }

  counter = 0;
  return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {

  if (n < 1) {
    printf("Error: Invalid time.\n");
    return 1;
  }

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;
  uint8_t bit_no_kbd, bit_no_timer;

  int erro = keyboard_subscribe(&bit_no_kbd); //subscribes the keyboard
  if (erro != OK) {
    printf("Error in keyboard_subscribe", 0);
    return erro;
  }

  erro = timer_subscribe_int(&bit_no_timer); //subscribes the timer
  if (erro != OK) {
    printf("Error in timer_subscribe_int", 0);
    return erro;
  }

  uint32_t irq_set_kbd = BIT(bit_no_kbd);
  uint32_t irq_set_timer = BIT(bit_no_timer);

  int ipc_status;
  message msg;

  while (scanByte != ESC_CODE && counter_t / 60 < n) { //Stops if ESC is pressed or has no input for n seconds
    /* Get a request message. */
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
        if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */
          kbc_ih();
          if (kbc_ih_error) {
            kbc_ih_error = false;
          } else {

            isTwoByte(&wait, &nbyte); //checks if the code is two bytes or not
            if (wait == false) {
              erro = scancode_parse(scanByte, nbyte); //calls kbd_print_scancode with the correct arguments
              if (erro != OK) 
                return erro;
            }

            counter_t = 0;
          }
          tickdelay (micros_to_ticks(DELAY_US));}
          break;

          default:
          break; /* no other notifications expected: do nothing */
        }

      }
      else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
    }

  erro = keyboard_unsubscribe(); //unsubscribes the keyboard
  if (erro != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return erro;
  }

  erro = timer_unsubscribe_int(); //unsibscribes the timer
  if (erro != OK) {
    printf("Error in timer_unsbscribe_int", 0);
    return erro;
  }

  return 0;
}