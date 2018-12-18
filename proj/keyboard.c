#include <lcom/lcf.h>

#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"

//global variable
static int hook_id = 0x01;
uint32_t scanByte = 0;
bool kbc_ih_error = false;

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

void (kbc_ih)(void) {

	uint32_t stat = 0;
	int numCiclos = 0;

	while (numCiclos < 5) {
		if (sys_inb(STAT_REG, &stat) != OK) {
			kbc_ih_error = true;
			return;
		}
		if (stat & OBF) {

			if (sys_inb(OUT_BUF, &scanByte) != OK) {
				kbc_ih_error = true;
				return;
			}

			if ((stat & (PAR_ERR | TO_ERR)) == 0) {
				kbc_ih_error = false;
				return;
			} else {
				kbc_ih_error = true;
				return;
			}
		}
		numCiclos++;
	}
	kbc_ih_error = true;
	return;
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

void (scancode_parse)(uint32_t byte, uint8_t nbyte) {

	uint8_t scancode[nbyte];

	if (nbyte == 2) {
		scancode[0] = TWO_BYTE_SCANCODE;
		scancode[1] = (uint8_t) byte;

	} else {
		scancode[0] = (uint8_t) byte;
	}

	return;
}

