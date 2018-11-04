#ifndef _MOUSE_H_
#define _MOUSE_H_


int (mouse_subscribe)(uint8_t * bit_no);
int (mouse_unsubscribe)();
int (mouse_enable)();
int (mouse_disable)();
int (write_kbc)(uint32_t cmd_byte);
int (OB_cleaner)();
void (mouse_handler)(void);
int (disable_mouse_interrupts)();
void (print_packet)();
int (set_remote_mode)();
int (set_stream_mode)();
int (enable_mouse_interrupts)();

extern uint32_t byte_array[3];
extern unsigned int byteNumber;

#endif //_MOUSE_H_
