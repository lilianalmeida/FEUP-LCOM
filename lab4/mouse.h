#ifndef _MOUSE_H_
#define _MOUSE_H_


int (mouse_subscribe)(uint8_t * bit_no);
int (mouse_unsubscribe)();
int (mouse_enable)();
int (mouse_disable)();
int (write_kbc)(uint32_t cmd_byte);
int (OB_cleaner)();
int (mouse_handler)(uint32_t *mouse_packet);

#endif //_MOUSE_H_
