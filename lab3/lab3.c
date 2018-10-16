  #include <lcom/timer.h>

  #include <stdbool.h>
  #include <stdint.h>

  #include "lab3.h"
  #include "keyboard.h"

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

int (kbd_test_scan)(bool UNUSED(assembly)) {

  if (assembly != 0 && assembly != 1){
    printf("Assembly not valid in kbd_test_scan", 0)
    return 1;
  }


  uint8_t bit_no = 1;

  int erro=keyboard_subscribe(&bit_no);
  if (erro != OK) {
    printf("Error in keyboard_subscribe", 0);
    return erro;
  }

  int ipc_status;
  message msg;
  uint32_t irq_set = bit_no;
  uint32_t scancode = 0;
  uint8_t nbyte = 0;

  while (scancode != ESC_CODE) {
    /* Get a request message. */
    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
      if (assembly == 0){
        keyboard_handler(&scancode, &nbyte);
        erro = scancode_parse (scancode, nbyte);
        //ver erro

      }
          if (assembly == 1) //something

        }
      tickdelay (micros_to_ticks (WAIT_KBC));
    }
    break;

    default:
        break; /* no other notifications expected: do nothing */
  }

    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
}



  if (assembly == 0){
    erro = kbd_print_no_sysinb (counter);
    //ver erro
  }

  erro=keyboard_unsubscribe();
  if (erro != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return erro;
  }

}


int (kbd_test_poll)() {
      /* To be completed */
}
int (kbd_test_timed_scan)(uint8_t UNUSED(n)) {
      /* To be completed */
      /* When you use argument n for the first time, delete the UNUSED macro */
}
