#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

typedef enum {SINGLE_PL, MULTI_PL, HIGHSCORE, EXIT} menu_options;
typedef enum {PLAYER1, PLAYER2, BACK} multiplayer_options;

void startMenu();
void parse_kbc_keys(uint8_t nbyte);
void call_func_menu();
void multiPlayerSelect();
void menuSelector( Bitmap *menu_back, Sprite *selector);

#endif
