#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t UNUSED(timer), uint32_t UNUSED(freq)) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *UNUSED(bit_no)) {
  /* To be completed by the students */


  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);
}


//// 7.1 ////
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	/* To be completed by the students */
	if (timer > 2 || timer < 0)
		return -1;

	uint32_t rbc = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

	int erro = sys_outb(TIMER_CTRL, rbc);
	if (erro != 0) {
		printf("Error in sys_outb");
		return erro;
	}

	uint32_t st32;

	erro = sys_inb(TIMER_0 + timer, &st32);
	if (erro != 0) {
		printf("Error in sys_inb");
		return erro;
	}

	*st = (uint8_t) st32;

	return 0;

}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
 

	union timer_status_field_val uni;
	
	
	
	switch(field)
    {
        case all:
            uni.byte = st;
            break;
        case initial:
            uni.byte = st >> 4 & (BIT(1) | BIT(0));
            break;
        case mode:
            uni.byte = st >> 1 & (BIT(2) | BIT(1) | BIT(0));
             break;
        case base:
            uni.byte = st & BIT(0);
            break;
    }	
	
	int erro = timer_print_config(timer, field, uni);
	if (erro != 0) {
		printf("Error in sys_inb");
		return erro;
	}
	
	
	
	
	/* To be completed by the students */
  return 0;
}
