#ifndef _MOUSE_TEST_H_
#define _MOUSE_TEST_H_

typedef enum {INIT, DRAW1, LINE1, VERTEX, DRAW2, LINE2, COMP} state_t;


int (mouse_subscribe)(uint8_t * bit_no);
int (mouse_unsubscribe)();
int (mouse_enable_data)();
int (mouse_disable_data)();
int (write_kbc)(uint32_t cmd_byte);
void (print_packet)();
int (set_stream_mode)();
int (enable_mouse_interrupts)();
void (gesture_handler)(struct mouse_ev *evt, uint8_t x_len);
void (mouse_events_handler)(struct mouse_ev *event, uint8_t x_len, uint8_t tolerance);

extern unsigned int byteNumber;
extern bool kbc_ih_error;
extern state_t state;

#endif //_MOUSE_TEST_H_
