#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


//// 7.2 ////
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
 
	if (timer > 2 || timer < 0)
		return 1;
	
	//if(freq < 0)
	//	return 1;

	uint32_t control_byte;
	uint8_t msb, lsb;
	
	uint32_t div = TIMER_FREQ/ freq;
	sys_inb(TIMER_CTRL,&control_byte);

	util_get_LSB(div, &lsb);
	util_get_MSB(div, &msb);

	control_byte = (control_byte & 0x0f) | TIMER_RB_SEL(timer) | TIMER_LSB_MSB;
	
	sys_outb(TIMER_CTRL, control_byte);
	sys_outb(TIMER_0 + timer, lsb);
	sys_outb(TIMER_0 + timer, msb);


  return 0;
}



//// 7.3 ////
int (timer_subscribe_int)(uint8_t *UNUSED(bit_no)) {
  /* to be completed by the students */


  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* to be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* to be completed by the students */
  printf("%s is not yet implemented!\n", __func__);
}



//// 7.1 ////
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	/* to be completed by the students */
	if (timer > 2 || timer < 0)
		return 1;

	uint32_t rbc = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_; //read back command

	int erro = sys_outb(TIMER_CTRL, rbc);
	if (erro != 0) {
		printf("error in sys_outb");
		return erro;
	}

	uint32_t st32; //

	erro = sys_inb(TIMER_0 + timer, &st32);
	if (erro != 0) {
		printf("error in sys_inb");
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
            uni.in_mode = (st & (BIT(5) | BIT(4))) >> 4;
            break;
        case mode:
            uni.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
             break;
        case base:
            uni.bcd = st & BIT(0);
            break;
    }	
	
	int erro = timer_print_config(timer, field, uni);
	if (erro != 0) {
		printf("error in print_config");
		return erro;
	}
	
	
  return 0;
}
