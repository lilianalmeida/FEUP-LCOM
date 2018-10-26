#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"



extern uint32_t scanByte;

//unsigned long kbc_asm_ih();

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

  if (assembly != 0 && assembly != 1){
    printf("Assembly not valid in kbd_test_scan", 0);
    return 1;
  }


  uint8_t bit_no;

  int erro=keyboard_subscribe(&bit_no);
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

          else if (!assembly){
            scanByte = 0x01;
            kbc_asm_ih();


            if (scanByte == 0x01)
            return -1;
          }
          isTwoByte(&wait, &nbyte);

          erro = scancode_parse (scanByte, nbyte);
          if(erro != OK)//ver erro
          return erro;

          tickdelay (micros_to_ticks (DELAY_US));

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

  if (!assembly){
    erro = kbd_print_no_sysinb (counter);
    return erro;
  }

  erro=keyboard_unsubscribe();
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
    if (kbc_pol(scanByte) == -1){
      continue;
    }

    isTwoByte(&wait, &nbyte);

    int erro = scancode_parse (scanByte, nbyte);
    if(erro != OK)//ver erro
    return erro;

    tickdelay (micros_to_ticks (DELAY_US));
  }

  if(kbd_print_no_sysinb (counter)){
    return !OK;
  }

  if (interrupt_handler() != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return -1;
  }

  return 0;
}

int (kbd_test_timed_scan)(uint8_t n) {

  uint8_t nbyte = 0; //numero de bytes do scancode
  bool wait = false;
  uint8_t bit_no_kbd, bit_no_timer;

  int erro=keyboard_subscribe(&bit_no_kbd);//subscribes the keyboard
  if (erro != OK) {
    printf("Error in keyboard_subscribe", 0);
    return erro;
  }

  erro=  timer_subscribe_int(&bit_no_timer);//subscribes the timer
  if (erro != OK) {
    printf("Error in timer_subscribe", 0);
    return erro;
  }

  uint32_t irq_set_kbd = BIT(bit_no_kbd);
  uint32_t irq_set_timer = BIT(bit_no_timer);
  printf("%x\n",bit_no_timer );
    printf("%x\n",bit_no_kbd );

  int ipc_status;
  message msg;

  while (scanByte != ESC_CODE && counter_t/60 < n) { //Stops if ESC is pressed or has no input for n seconds
    /* Get a request message. */
    printf("%x\n", counter_t);
    if ((erro = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver_receive failed with: %d", erro);
      continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */

      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set_timer){/* subscribed interrupt */
          timer_int_handler();
          printf("cenas\n",0 );
        }
        if (msg.m_notify.interrupts & irq_set_kbd){ /* subscribed interrupt */
          kbc_ih();
          printf("cenas2\n",0 );
          isTwoByte(&wait, &nbyte);//checks if the code is two bytes or not

          erro = scancode_parse (scanByte, nbyte);//calls kbd_print_scancode with the correct arguments
          if(erro != OK)//ver erro
          return erro;

          //counter_t =0;
          tickdelay (micros_to_ticks (DELAY_US));
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


  erro = kbd_print_no_sysinb (counter);//prints the number of sys_inb() calls
  if (erro != OK) {
    printf("Error in kbd_print_no_sysinb", 0);
    return erro;
  }

  erro=keyboard_unsubscribe();//unsubscribes the keyboard
  if (erro != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return erro;
  }

  erro=timer_unsubscribe_int();//unsibscribes the timer
  if (erro != OK) {
    printf("Error in keyboard_unsbscribe", 0);
    return erro;
  }

  counter = 0;//resets the timer variable for the next function call
  return 0;
}




void (kbc_ih)(void){

  uint32_t stat = 0;
  int numCiclos = 0;

  while(numCiclos < 5) {
    printf("Num %x\n",numCiclos );
    sys_inb_count(STAT_REG, &stat);


    /* assuming it returns OK */ //verificar eroo??
    /* loop while 8042 output buffer is empty */
    if( stat & OBF ) {

      sys_inb_count(OUT_BUF, &scanByte); /* assuming it returns OK */ //verificar eroo??#else

      if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
        return;
      }
      numCiclos++;
    }
  }
}
