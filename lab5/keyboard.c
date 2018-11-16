#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "keyboard.h"

//global variable
static int hook_id = 0x01;
uint32_t counter =0;
uint32_t scanByte = 0;

int (sys_inb_count) (port_t port, uint32_t *byte){

	counter++;
	return sys_inb (port,byte);
}



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


int (kbc_pol)() {
	uint32_t stat = 0;
  	int numCiclos = 0;

  while(numCiclos <= 5) {

    sys_inb_count(STAT_REG, &stat);

    if( stat & OBF ) {

      sys_inb_count(OUT_BUF, &scanByte);

      if ( (stat &(PAR_ERR | TO_ERR | AUX)) != 0 ){
        return -1;
      }
      else
        return 0;
    }
    numCiclos++;
  }
  return -1;
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

int (interrupt_handler)(){
	uint32_t cmd;

	sys_outb(KBC_CMD_REG, READ_COMMAND);
	sys_inb(OUT_BUF, &cmd);

	cmd |= INTR_ENABLE;

	sys_outb(KBC_CMD_REG, WRITE_COMMAND);
	sys_outb(KBC_CMD_REG, cmd);

	return 0;

}


void (isTwoByte)(bool *wait, uint8_t *nbyte){

	if (*wait == false){
		if (scanByte == TWO_BYTE_SCANCODE){
			*wait = true;
			return;
		}
		else{
			*nbyte = 1;
			*wait = false;
		}
	}

	else{
		*nbyte = 2;
		*wait = false;
	}
}
