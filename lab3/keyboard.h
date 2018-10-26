#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "macro.h"

//global variable
extern uint32_t counter; //sys_inb counter

//int sys_inb_count (port_t port, uint32_t *byte);
int (keyboard_subscribe)(uint8_t * bit_no);
int (keyboard_unsubscribe)();
//int (keyboard_handler)(uint32_t *byte);
int (scancode_parse)(uint32_t byte, uint8_t nbyte);




