#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
#include "video_gr.h"

static int hook_id = 0x03;
unsigned int byteNumber = 0;
static uint32_t byte_array[3];
bool mouse_ih_error = false;
uint32_t counter_t = 0;
static struct packet pp;

int mouse_subscribe(uint8_t * bit_no) {

	*bit_no = hook_id;

	if(sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&hook_id) != OK){
		printf("Error subscribing mouse\n");
		return 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int mouse_unsubscribe() {

	int erro = sys_irqrmpolicy(&hook_id);
	if (erro != OK) {
		printf("Error in sys_irqrmpolicy", 0);
		return erro;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int mouse_enable_data(){
	int counter =0;
	while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries
		if(write_kbc(MOUSE_ENABLE) == 0)
		return 0;
		counter++;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int mouse_disable_data(){
	int counter =0;
	while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries
		if(write_kbc(MOUSE_DISABLE)== 0)
		return 0;

		counter++;
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int write_kbc(uint32_t cmd_byte){
	uint32_t status = 0;
	int done = 0;
	uint32_t verification;
	while(!done){
		if (sys_inb(STAT_REG, &status) != OK) return -1; // verify the status of the buffer

		if ((status & IBF) == 0) {
			sys_outb(KBC_CMD_REG, KBC_CMD_INIT); //allows the mouse to receive commands directly
		}
		if (sys_inb(STAT_REG, &status) != OK) return -1; // verify the status of the buffer

		if ((status & IBF) == 0) {
			sys_outb(IN_BUFF,cmd_byte); //writes the command byte
		}
		sys_inb(OUT_BUFF,&verification);
		if(verification == NACK){
			continue;
		}
		else if(verification == ERROR){
			return 1;
		}
		else{
			done= 1;
		}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void (mouse_ih) (){

	uint32_t byte;

	if(sys_inb(OUT_BUFF,&byte) != 0){//reads output buffer
		printf("Error reading output buffer\n",0 );
		mouse_ih_error = true;
		return;
	}

	byte_array[byteNumber] = byte; //sends the byte read to the array
	byteNumber++;

	if(!((MOUSE_BIT3 & byte_array[0])>> 3)){ //tests bit(3) of the first byte
		printf("Error reading mouse packet\n",0 );
		mouse_ih_error = true;
		return;
	}

	mouse_ih_error = false;
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void parse_packet(){
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
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int set_stream_mode(){
	if(write_kbc(SET_STREAM)== 0)
		return 0;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int OB_cleaner(){
	uint32_t trash, status;
    if(sys_inb(STAT_REG,&status) != 0) //reads the current status of the output buffer to get OBF byte
        return 1;
    if(status & OBF){
        sys_inb(OUT_BUFF,&trash);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void move_aim (Sprite *aim){
	aim->x += pp.delta_x;
	aim->y -= pp.delta_y;
	if (aim->y <= 70 ){
		aim->y = 71;
	}else if ((aim->y + aim->height)> (uint32_t) getVerResolution()){
		aim->y += pp.delta_y;
	}
	if (aim-> x < 0){
		aim->x = 0;
	}else if ((aim->x + aim->width) > (uint32_t) getHorResolution()){
		aim->x -= pp.delta_x;
	}
}

bool is_left_pressed(){
	return pp.lb;
}
