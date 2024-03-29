#include <lcom/lcf.h>
#include <lcom/timer.h>
#include "mouse_test.h"
#include "macros.h"
#include "i8254.h"

static int hook_id = 0x01;
unsigned int byteNumber = 0;
static uint32_t byte_array[3];
bool kbc_ih_error = false;
uint32_t counter_t = 0;
state_t state = INIT;
static struct packet pp;

int (mouse_subscribe)(uint8_t * bit_no) {

	*bit_no = hook_id;

	if(sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&hook_id) != OK){
		printf("Error subscribing mouse\n");
		return 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_unsubscribe)() {

	int erro = sys_irqrmpolicy(&hook_id);
	if (erro != OK) {
		printf("Error in sys_irqrmpolicy", 0);
		return erro;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (mouse_enable_data)(){
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

int (mouse_disable_data)(){
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

int (write_kbc)(uint32_t cmd_byte){
	uint32_t status = 0;
	int done = 0;
	uint32_t verification;
	while(!done){
		if (sys_inb(STAT_REG, &status) != OK) return -1;

		if ((status & IBF) == 0) {
			sys_outb(KBC_CMD_REG, KBC_CMD_INIT); //prepares mouse for writing
		}
		if (sys_inb(STAT_REG, &status) != OK) return -1;

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

void (mouse_ih)(void){

	uint32_t byte;

	if(sys_inb(OUT_BUFF,&byte) != 0){//reads output buffer
		printf("Error reading output buffer\n",0 );
		kbc_ih_error = true;
		return;
	}

	byte_array[byteNumber] = byte; //sends the byte read to the array
	byteNumber++;

	if(!((MOUSE_BIT3 & byte_array[0])>> 3)){ //tests bit(3) of the first byte
		printf("Error reading mouse packet\n",0 );
		kbc_ih_error = true;
		return;
	}

	kbc_ih_error = false;
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void (print_packet)(){
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

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (set_stream_mode)(){
		if(write_kbc(SET_STREAM)== 0)
		return 0;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

int (enable_mouse_interrupts)() {
	uint32_t cmd;
	uint32_t stat = 0;
	int numCiclos = 0;

	cmd = minix_get_dflt_kbc_cmd_byte();

	while (numCiclos < 5) {
		if (sys_inb(STAT_REG, &stat) != 0)
		return 1; 

		if ((stat & IBF) == 0) {
			if (sys_outb(KBC_CMD_REG, WRITE_COMMAND) != OK) {
				return 1;
			}
			else{
				numCiclos = 0;
				break;
			}
		}
		numCiclos++;
	}

	while (numCiclos < 5) {
		if (sys_inb(STAT_REG, &stat) != 0)
		return 1; 

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

void (mouse_events_handler)(struct mouse_ev *event,uint8_t x_len, uint8_t tolerance){
	if (!pp.rb && pp.lb && !pp.mb){ //if left button is pressed
              if (pp.delta_x == 0 && pp.delta_y == 0){
                event->type = LB_PRESSED;
                gesture_handler(event, x_len);
                return;
              }
              event->delta_x += pp.delta_x;
              event->delta_y += pp.delta_y;

              if ((pp.delta_x >= -tolerance) && (pp.delta_y >= -tolerance)){ //displacements in x and y
                if (event->delta_y / event->delta_x > 0){ //positive slope
                  event->type = MOUSE_MOV;
                  gesture_handler(event, x_len);
                }
                else{
                  event->type = BUTTON_EV;
                  gesture_handler(event, x_len);
                }
              }else{
                event->type = BUTTON_EV;
                gesture_handler(event, x_len);

              }
            }else if (pp.rb && !pp.lb && !pp.mb){ //if rigth button is pressed
              if(pp.delta_x == 0 && pp.delta_y == 0){
                event->type = RB_PRESSED;
                gesture_handler(event, x_len);
                return;
              }
              event->delta_x += pp.delta_x;
              event->delta_y += pp.delta_y;
              if ((pp.delta_x >= -tolerance) && (pp.delta_y <= tolerance)){ //displacements in x and y
                if (event->delta_y / event->delta_x < 0){ //positive slope
                  event->type = MOUSE_MOV;
                  gesture_handler(event, x_len);
                }
                else{
                  event->type = BUTTON_EV;
                  gesture_handler(event, x_len);
                }
              }else{
                event->type = BUTTON_EV;
                gesture_handler(event, x_len);
              }

            }else if (!pp.rb && !pp.lb && !pp.mb){ //if no button is pressed
              if (state == LINE1 || state == DRAW1){
                event->type = LB_RELEASED;
              }else if (state == LINE2 || state == DRAW2){
                event->type = RB_RELEASED;
              }
              else if(state == VERTEX) {
                if ((abs(pp.delta_x) <= tolerance) && (abs(pp.delta_y) <= tolerance)){
                  event->type = MOUSE_MOV;
                }
                else{
                  event->type = BUTTON_EV;
                }

              }
              else
              event->type = BUTTON_EV;

              gesture_handler(event, x_len);
            }

            else{ //if the middle button is pressed or more than one button is pressed
              event->type = BUTTON_EV;
              gesture_handler(event, x_len);
            }

}

void (gesture_handler)(struct mouse_ev *evt, uint8_t x_len) {
	switch (state) {
		case INIT:
		if( evt->type == LB_PRESSED )
		state = DRAW1;
		break;
		case DRAW1:
		if( evt->type == MOUSE_MOV ) {
			if ((evt->delta_x >= x_len) && (evt->delta_y/evt->delta_x > 1)){
				state = LINE1;
			}

		} else if( evt->type == LB_RELEASED || evt->type == RB_PRESSED || evt->type == BUTTON_EV){
			state = INIT;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}

		break;
		case LINE1:
		if( evt->type == LB_RELEASED ){
			state = VERTEX;
		}else if (evt->type == MOUSE_MOV){
			state = DRAW1;
		}else if(evt->type == RB_PRESSED || evt->type == BUTTON_EV){
			state = INIT;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}

		break;
		case VERTEX:
		if( evt->type == RB_PRESSED ){
			state = DRAW2;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}else if(evt->type == LB_PRESSED){
			state = DRAW1;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}else if(evt->type == BUTTON_EV){
			state = INIT;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}
		break;
		case DRAW2:
		if( evt->type == MOUSE_MOV ) {
			if ((evt->delta_x >= x_len) && (abs(evt->delta_y/evt->delta_x) > 1)){
				state = LINE2;
			}

		} else if( evt->type == RB_RELEASED || evt->type == LB_PRESSED || evt->type == BUTTON_EV){
			state = INIT;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}
		break;
		case LINE2:
		if( evt->type == RB_RELEASED ){
			state = COMP;
		}
		if (evt->type == MOUSE_MOV){
			state = DRAW2;
		}else if(evt->type == LB_PRESSED || evt->type == BUTTON_EV){
			state = INIT;
			evt->delta_x = 0;
			evt->delta_y = 0;
		}

		break;
		default:
		break;
	}
}
