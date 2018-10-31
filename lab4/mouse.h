#ifndef _MOUSE_H_
#define _MOUSE_H_


int (mouse_subscribe)(int * mouse_id);
int (mouse_unsubscribe)(int * mouse_id);
int (mouse_enable)();
int (write_kbc)(uint32_t cmd_byte);
int (OB_cleaner)();

#endif
