#include <lcom/lcf.h>

#include <lcom/timer.h>
#include <stdint.h>
#include "keyboard.h"

//global variable
int keyboard_id = 0x01;
uint32_t counter =0;

int sys_inb_count (port_t port, uint32_t *byte){

	counter++;
	return sys_inb (port,byte);
}



int (keyboard_subscribe)(uint8_t * bit_no) {

	int erro = sys_irqsetpolicy(KEYBOARD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &keyboard_id);
	if (erro != OK) {
		printf("Error in sys_irqsetpolicy", 0);
		return erro;
	}

	*bit_no = keyboard_id;

	return 0;
}

int (keyboard_unsubscribe)() {

	int erro = sys_irqrmpolicy(&keyboard_id);
	if (erro != OK) {
		printf("Error in sys_irqrmpolicy", 0);
		return erro;
	}

	return 0;
}


int (scancode_parse)(uint32_t byte, uint8_t nbyte){

	uint8_t scancode [nbyte];

	if (nbyte == 2){
		scancode [0] = TWO_BYTE_SCANCODE;
		scancode [1] = (uint8_t) byte;

	}
	else
	{
		scancode [0] = (uint8_t) byte;
	}

	int erro = kbd_print_scancode (!((BIT(7) & byte) >> 7), nbyte, scancode);
		if (erro != OK){
			printf("Error in kbd_print_scancode", 0);
			return erro;
		}

return 0;
}

