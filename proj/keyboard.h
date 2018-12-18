#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//global variable
extern bool kbc_ih_error;
extern uint32_t scanByte;

int keyboard_subscribe(uint8_t * bit_no);
int keyboard_unsubscribe();
void scancode_parse(uint32_t byte, uint8_t nbyte);
void isTwoByte(bool *wait, uint8_t *nbyte);


#endif
