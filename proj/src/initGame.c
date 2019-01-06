#include <lcom/lcf.h>

#include "initGame.h"
#include "keyboard.h"
#include "mouse.h"
#include "vbe_macros.h"
#include "rtc.h"
#include "serial_port.h"

static uint32_t timer_irq_set;
static uint32_t kbc_irq_set;
static uint32_t mouse_irq_set;
static uint32_t rtc_irq_set;
static uint32_t uart_irq_set;

uint32_t getKBC_IRQ() {
  return kbc_irq_set;
}

uint32_t getTIMER_IRQ() {
  return timer_irq_set;
}

uint32_t getMOUSE_IRQ() {
  return mouse_irq_set;
}
uint32_t getRTC_IRQ() {
  return rtc_irq_set;
}
uint32_t getUART_IRQ(){
  return uart_irq_set;
}

int devices_init() {
  uint8_t timer_bit_no, kbc_bit_no, mouse_bit_no, rtc_bit_no, uart_bit_no;

  if(keyboard_subscribe(&kbc_bit_no) != OK){
    printf("Error enabling keyboard interrupts",0);
    return 1;
  }
  if(timer_subscribe_int(&timer_bit_no) != OK){
    printf("Error enabling timer interrupts",0);
    return 1;
  }

  if (rtc_subscribe(&rtc_bit_no) != OK){
    return 1;
  }

  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_enable_data() != OK){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

  if(mouse_subscribe(&mouse_bit_no) != OK){
    printf("Error subscribing mouse notifications\n");
    return 1;
  }

  if(serialPort_subscribe(&uart_bit_no) != OK){
    printf("Error enabling serial port interrupts",0);
    return 1;
  }

  kbc_irq_set = BIT(kbc_bit_no);
  timer_irq_set = BIT(timer_bit_no);
  mouse_irq_set = BIT(mouse_bit_no);
  rtc_irq_set = BIT(rtc_bit_no);
  uart_irq_set = BIT(uart_bit_no);

  if(vg_init(MODE117) == NULL){
    printf("Error setting graphics mode\n");
    return 1;
  }

  return 0;
}


int devices_end() {

  if (timer_unsubscribe_int() != OK) {
    printf("Error disabling timer interrupts\n");
    return 1;
  }

  if (keyboard_unsubscribe() != OK) {
    printf("Error disabling keyboard interrupts\n");
    return 1;
  }

  if (rtc_unsubscribe() != OK){
    printf("Error disabling rtc interrupts\n");
    return 1;
  }

  if(mouse_unsubscribe() != OK){
    printf("The program was unable to unsubscribe a mouse notification\n");
    return 1;
  }

  if(mouse_disable_data() != OK){
    printf("Error disabling mouse data reporting\n");
    return 1;
  }
  if(serialPort_unsubscribe() != OK){
    printf("Error disabling serial port interrupts\n");
    return 1;
  }

  vg_exit();

  return 0;
}
