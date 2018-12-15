#ifndef _MOUSE_TEST_H_
#define _MOUSE_TEST_H_

#include "bitmap.h"

//typedef enum {STILL, RELEASED, PRESSED} left_button;

int mouse_subscribe(uint8_t * bit_no);
int mouse_unsubscribe();
int mouse_enable_data();
int mouse_disable_data();
int write_kbc(uint32_t cmd_byte);
void parse_packet();
int set_stream_mode();
int OB_cleaner();
void move_aim (Sprite *aim);
bool is_left_pressed();

extern unsigned int byteNumber;
extern bool mouse_ih_error;

#endif //_MOUSE_TEST_H_
