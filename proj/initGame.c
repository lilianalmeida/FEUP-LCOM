#include <lcom/lcf.h>

#include "keyboard.h"
#include "mouse_test.h"
#include "vbe_macros.h"
#include "initGame.h"

static uint32_t timer_irq_set;
static uint32_t kbc_irq_set;
static uint32_t mouse_irq_set;

uint32_t getKBC_IRQ() {
  return kbc_irq_set;
}

uint32_t getTIMER_IRQ() {
  return timer_irq_set;
}

uint32_t getMOUSE_IRQ() {
  return mouse_irq_set;
}

int devices_init() {
  uint8_t timer_bit_no, kbc_bit_no, mouse_bit_no;

  if(keyboard_subscribe(&kbc_bit_no) != OK){
    printf("Error enabling keyboard interrupts",0);
    return 1;
  }
  if(timer_subscribe_int(&timer_bit_no) != OK){
    printf("Error enabling timer interrupts",0);
    return 1;
  }


  if(set_stream_mode() != OK){
    return 1;
  }

  if(mouse_enable_data() != 0){
    printf("The program failed to enable the mouse data reporting\n");
    return 1;
  }

  if(mouse_subscribe(&mouse_bit_no) != 0){
    printf("Error subscribing mouse notifications\n");
    return 1;
  }

  kbc_irq_set = BIT(kbc_bit_no);
  timer_irq_set = BIT(timer_bit_no);
  mouse_irq_set = BIT(mouse_bit_no);

  if(vg_init(MODE117) == NULL){
    printf("Error setting graphics mode\n");
    return 1;
  }

  return 0;
}


int devices_end() {
  
  if (timer_unsubscribe_int() != 0) {
    printf("Error disabling timer interrupts\n");
    return 1;
  }

  if (keyboard_unsubscribe() != 0) {
    printf("Error disabling keyboard interrupts\n");
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

  vg_exit();
  printf("Set to text mode\n");

  return 0;
}

