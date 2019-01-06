#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

//global variables
static int hook_id = 0x01;

int (keyboard_subscribe)(uint8_t * bit_no) {

	*bit_no = hook_id;

	int erro = sys_irqsetpolicy(KEYBOARD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id);
	if (erro != OK) {
		printf("Error in sys_irqsetpolicy", 0);
		return erro;
	}
	return 0;
}

int (keyboard_unsubscribe)() {

	int erro = sys_irqrmpolicy(&hook_id);
	if (erro != OK) {
		printf("Error in sys_irqrmpolicy", 0);
		return erro;
	}

	return 0;
}

void (isTwoByte)(bool *wait, uint8_t *nbyte) {

	if (*wait == false) {
		if (scanByte == TWO_BYTE_SCANCODE) {
			*wait = true;
			return;
		} else {
			*nbyte = 1;
			*wait = false;
		}
	}

	else {
		*nbyte = 2;
		*wait = false;
	}
}

