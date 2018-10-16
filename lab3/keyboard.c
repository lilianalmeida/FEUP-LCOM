#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "keyboard.h"

//global variable
int keyboard_id = 0x01;
uint32_t counter =0;

int sys_inb_count (port_t port, uint32_t *byte){

	#define LAB3
	int erro = sys_inb (port,byte);
	#ifdef LAB3
	counter++;
	#endif
}



int (keyboard_subscribe)(uint8_t * bit_no) {

	int erro = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE | IQR_EXCLUSIVE, &keyboard_id);
	if (erro != OK) {
		printf("Error in sys_irqsetpolicy", 0);
		return erro;
	}

	*bit_no = BIT(keyboard_id);

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

int (keyboard_handler)(uint32_t *scancode, uint8_t *nbyte) { //reads the bytes from the KBC's OUT_BUF (one byte per interrupt)

	while( stat & OBF ) {
	sys_inb_count(STAT_REG, &stat); /* assuming it returns OK */
							/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb_count(OUT_BUF, scancode); /* assuming it returns OK */
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 ){
				if ((*scancode && TWO_BYTE_SCANCODE)==TWO_BYTE_SCANCODE){
					*nbyte = 2;
					return 0;
				}
				else {
					*nbyte = 1;
					return 0;
				}
			}
			else
				return 1;
		}
		delay(WAIT_KBC);
	}
	return 0;
}

int (scancode_parse)(uint32_t scancode, uint8_t nbyte){

	int erro;
	uint8_t byte [nbyte];
	uint8_t first_byte = (uint8_t) scancode;
	byte [0] = first_byte;

	if (nbyte == 2){
		uint8_t second_byte = (uint8_t) (scancode >> 8);
		byte [1] = second_byte;
		erro = kbd_print_scancode (bool (BIT(15) & scancode), nbyte, byte)
		if (erro != OK){
			printf("Error in kbd_print_scancode", 0);
			return erro;
		}
	}
	else
		erro = kbd_print_scancode (bool (BIT(7) & scancode), nbyte, byte);
	if (erro != OK){
			printf("Error in kbd_print_scancode", 0);
			return erro;
		}

return 0;
}



