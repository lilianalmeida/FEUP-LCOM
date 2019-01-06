#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//global variable
static int hook_id_t = 0x02;

//// 7.2 ////
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

	uint8_t control_byte;
	uint8_t msb, lsb, st;

	if(freq >= TIMER_FREQ || freq < 19){
		printf("Frequency not valid. ",0);
		return 1;
	}

	int erro = timer_get_conf(timer, &st); //read the timer configuration
	if (erro != OK) {
		printf("Error in timer_get_conf", 0);
		return erro;
	}

	control_byte = (st & 0x0f) | (timer << 6) | TIMER_LSB_MSB; //control word to configure the timer

	erro = sys_outb(TIMER_CTRL, control_byte); //write control word to configure timer
	if (erro != OK) {
		printf("Error in sys_outb", 0);
		return erro;
	}

	uint16_t div = TIMER_FREQ / freq;

	util_get_LSB(div, &lsb);
	util_get_MSB(div, &msb);

	erro = sys_outb(TIMER_0 + timer, lsb); //write lsb of the frequency desired first
	if (erro != OK) {
		printf("Error in sys_outb", 0);
		return erro;
	}

	erro = sys_outb(TIMER_0 + timer, msb); //write msb of the frequency desired then
	if (erro != OK) {
		printf("Error in sys_outb", 0);
		return erro;
	}

	return 0;
}


//// 7.3 ////
int (timer_subscribe_int)(uint8_t * bit_no) {

	*bit_no = hook_id_t;

	int erro = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_t);
	if (erro != OK) {
		printf("Error in sys_irqsetpolicy", 0);
		return erro;
	}

	return 0;
}

int (timer_unsubscribe_int)() {

	int erro = sys_irqrmpolicy(&hook_id_t);
	if (erro != OK) {
		printf("Error in sys_irqrmpolicy", 0);
		return erro;
	}

	return 0;
}

void (timer_int_handler)() {
	counter_t++;
}


//// 7.1 ////
int (timer_get_conf)(uint8_t timer, uint8_t *st) {

	uint32_t rbc = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_; //read back command

	int erro = sys_outb(TIMER_CTRL, rbc);
	if (erro != OK) {
		printf("Error in sys_outb");
		return erro;
	}

	uint32_t st32; //

	erro = sys_inb(TIMER_0 + timer, &st32);
	if (erro != OK) {
		printf("Error in sys_inb");
		return erro;
	}

	*st = (uint8_t) st32;

	return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

	union timer_status_field_val uni;

	switch (field) {
		case all:
		uni.byte = st;
		break;

		case initial:
		uni.in_mode = (st & (BIT(5) | BIT(4))) >> 4;
		break;

		case mode:
		uni.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
		if (uni.count_mode == 0x06) //when 110 -> operating mode is 2
		uni.count_mode = 0x02;

		if (uni.count_mode == 0x07) //when 111 -> operating mode is 3
		uni.count_mode = 0x03;
		break;

		case base:
		uni.bcd = st & BIT(0);
		break;
		
		default:
		printf("timer_status_field not recognized",0);
	}

	int erro = timer_print_config(timer, field, uni);
	if (erro != OK) {
		printf("Error in print_config");
		return erro;
	}

	return 0;
}
