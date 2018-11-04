#include <lcom/lcf.h>
#include "mouse.h"
#include "macros.h"

static int hook_id = 0x01;
unsigned int byteNumber = 0;
uint32_t byte_array[3];

int (mouse_subscribe)(uint8_t * bit_no) {

*bit_no = hook_id;

	if(sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&hook_id) != OK){
		printf("Error subscribing mouse\n");
		return 1;
	}

	return 0;
}

int (mouse_unsubscribe)() {

	int erro = sys_irqrmpolicy(&hook_id);
		if (erro != OK) {
			printf("Error in sys_irqrmpolicy", 0);
			return erro;
		}
		return 0;
}

	int (mouse_enable)(){
		int counter =0;
		while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries
			if(write_kbc(KBC_CMD_INIT)==0 && write_kbc(MOUSE_ENABLE)== 0)
			return 0;
			counter++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
		return 1;
	}

	int (mouse_disable)(){
		int counter =0;
		while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries
			if(write_kbc(KBC_CMD_INIT)==0 && write_kbc(MOUSE_DISABLE)== 0)
			return 0;
			counter++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
		return -1;
	}


	int (write_kbc)(uint32_t cmd_byte){

		int cnt = 0;
		uint32_t verificationBits;
		while(cnt < 5){ //Loops 5 times before exiting without succes
			sys_outb(KBC_CMD_REG, KBC_CMD_INIT); //prepares mouse for writing
			sys_outb(IN_BUFF,cmd_byte); //writes the command byte
			tickdelay(micros_to_ticks(DELAY_US));
			sys_inb(OUT_BUFF,&verificationBits);

			if (verificationBits != NACK || verificationBits != ERROR){
				break;//if there are no erros breaks the loop and returns 0
			}
			tickdelay(micros_to_ticks(DELAY_US));
			cnt++;
		}

		if(cnt == 5){ //returns 1 if the previous loop failed 5 times
			return 1;
		}

		return 0;
	}

	int (OB_cleaner)(){

		uint32_t trash, status;

		if(sys_inb(STAT_REG,&status) != 0) //reads the current status of the output buffer to get OBF byte
			return 1;

		while(status & OBF) //while the output buffer is full sends that date to the trash variable to clean the OB
			sys_inb(OUT_BUFF,&trash);

		return 0;
		}


void (mouse_handler)(void){

	uint32_t	byte;

	if(sys_inb(OUT_BUFF,&byte) != 0){//reads output buffer
		printf("Error reading output buffer\n",0 );
		byteNumber = 4;
	}

	byte_array[byteNumber] = byte; //sends the byte read to the array
	byteNumber++;

	if(!(MOUSE_BIT3 & byte_array[0])){ //tests bit(3) of the first byte
		printf("Error readind mouse packet\n",0 );
		byteNumber = 4;
	}

	if(byteNumber > 2)
	byteNumber = 0;

}

void (print_packet)(){
	struct packet pp;
    pp.bytes[0] = byte_array[0];
    pp.bytes[1] = byte_array[1];
    pp.bytes[2] = byte_array[2];
    pp.rb = (byte_array[0] & BIT(1)) >> 1;
    pp.lb = (byte_array[0] & BIT(0));
    pp.mb = (byte_array[0] & BIT(2)) >> 2;
    pp.x_ov = (byte_array[0] & BIT(6)) >> 6;
    pp.y_ov = (byte_array[0] & BIT(7)) >> 7;
    if ((byte_array[0] & BIT(4)) == 0){
      pp.delta_x = 0x00FF & byte_array[1];
    }
    else{
      pp.delta_x = 0xFF00 | byte_array[1];
    }
     if ((byte_array[0] & BIT(5)) == 0){
      pp.delta_y = 0x00FF & byte_array[2];
    }
    else{
      pp.delta_y = 0xFF00 | byte_array[2];
    }

    mouse_print_packet(&pp);
}
int (disable_mouse_interrupts)() {
	uint32_t cmd;
	uint32_t stat = 0;
	int numCiclos = 0;

	if (sys_outb(KBC_CMD_REG, READ_COMMAND) != OK) {
		return 1;
	}

	if (sys_inb(OUT_BUFF, &cmd) != OK) {
		return 1;
	}

	cmd = cmd & MOUSE_DIS_CMD;

	if (sys_outb(KBC_CMD_REG, WRITE_COMMAND) != OK) {
		return 1;
	}

	while (numCiclos < 5) {
		if (sys_inb(STAT_REG, &stat) != 0)
			return 1; //assuming it returns OK

		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, cmd) != OK) {
				return 1;
			} else {
				return 0;
			}
		}
		numCiclos++;
	}

	return 1;

}


int (set_remote_mode)(){
	int counter =0;
		while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries

			if(write_kbc(SET_REMOTE)== 0)
			return 0;
			counter++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
	return 1;
}

int (set_stream_mode)(){
	int counter =0;
		while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries

			if(write_kbc(SET_STREAM)== 0)
			return 0;
			counter++;
			tickdelay(micros_to_ticks(DELAY_US));
		}
	return 1;
}


int (enable_mouse_interrupts)() {
	uint32_t cmd;
	uint32_t stat = 0;
	int numCiclos = 0;

	if (sys_outb(KBC_CMD_REG, READ_COMMAND) != OK) {
		return 1;
	}

	if (sys_inb(OUT_BUFF, &cmd) != OK) {
		return 1;
	}

	cmd = minix_get_dflt_kbc_cmd_byte();

	if (sys_outb(KBC_CMD_REG, WRITE_COMMAND) != OK) {
		return 1;
	}

	while (numCiclos < 5) {
		if (sys_inb(STAT_REG, &stat) != 0)
			return 1; //assuming it returns OK

		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, cmd) != OK) {
				return 1;
			} else {
				return 0;
			}
		}
		numCiclos++;
	}

	return 1;

}
