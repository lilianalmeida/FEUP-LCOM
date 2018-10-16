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

	int erro = sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_id);
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

int (keyboard_handler)(uint32_t *byte) { //reads the bytes from the KBC's OUT_BUF (one byte per interrupt)
	uint32_t stat = 0;
	
	while(1) {
		#ifdef LAB3
		sys_inb_count(STAT_REG, &stat);
		#else
		#define sys_inb_cnt(p,q) sys_inb(p,q)
		#endif

		sys_inb_count(STAT_REG, &stat); /* assuming it returns OK */ //verificar eroo??
							/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			#ifdef LAB3
			sys_inb_count(OUT_BUF, byte); /* assuming it returns OK */ //verificar eroo??#else
			#define sys_inb_cnt(p,q) sys_inb(p,q)
			#endif

			if ( (stat &(PAR_ERR | TO_ERR)) != 0 ){
				return 1;
			}
			/*else
				return 1;*/
		}
		tickdelay (micros_to_ticks (WAIT_KBC));
		//delay(WAIT_KBC);
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

	int erro = kbd_print_scancode ((BIT(7) & byte) >> 7, nbyte, scancode);
		if (erro != OK){
			printf("Error in kbd_print_scancode", 0);
			return erro;
		}

return 0;
}

