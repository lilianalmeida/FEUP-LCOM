#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

typedef enum {SINGLE_PL, MULTI_PL, HIGHSCORE, EXIT} menu_options;

void start_main_menu ();
void parse_kbc_keys(Sprite *selector, Bitmap *menu_back, uint8_t nbyte);
void call_func_menu();

#endif
