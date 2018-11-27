#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#ifdef LAB3
int sys_inb_count(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

//global variable
extern uint32_t counter; //sys_inb counter
extern bool kbc_ih_error;
extern uint32_t scanByte;

int (keyboard_subscribe)(uint8_t * bit_no);
int (keyboard_unsubscribe)();
int (kbc_pol)();
int (scancode_parse)(uint32_t byte, uint8_t nbyte);
int (interrupt_handler)();
void (isTwoByte)(bool *wait, uint8_t *nbyte);


#endif
