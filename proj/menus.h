#ifndef _MENUS_H_
#define _MENUS_H_

#include "bitmap.h"

/** @defgroup Menus Menus
* @{
* Functions for manipulating multiplayer and main menu
*/

///Specifies the option selected in the main menu
typedef enum {
	SINGLE_PL, /**< @brief Play in singleplayer mode option*/
	MULTI_PL, /**< @brief Play in multiplayer mode option*/
	HIGHSCORE, /**< @brief See highscores option*/
	EXIT /**< @brief Exit game option*/
} menu_options;

///Specifies the option selected in the multiplayer menu
typedef enum {
	PLAYER1, /**< @brief Play as player 1 option*/
	PLAYER2, /**< @brief Play as player 2 option*/
	BACK /**< @brief Exit multiplayer menu option*/
} multiplayer_options;

/**
 * @brief Manipulates main menu
 * 
 * Draws the necessary bitmaps, reads the keyboard keys and according to them navigates through the menu with the possibility to play in single or multiplayer mode, see the highscores anda exit the game.
 * Reads mouse packets and discards them
 */
void startMenu();

/**
 * @brief Manipulates multiplayer menu
 * 
 * Draws the necessary bitmaps, reads the keyboard keys and according to them navigates through the menu with the possibility to play the multiplayer game as player 1 or player 2 or return to main menu
 * Reads mouse packets and discards them
 */
void multiPlayerSelect();

/**
 * @brief According to the keyboard key read, change the corresponding menu option
 *
 * @param nbyte number of bytes of the last keyboard key read
 */
void parse_kbc_keys(uint8_t nbyte);

/**
 * @brief Draws the menu with the current option selected
 * 
 * Draw the menu background and a rectangle at a determined position so that it selects a option, which corresponds to the current element in the menu enumeration
 * @param menu_back bitmap of the menu background
 * @param selector sprite of the rectangle that selects the option
 */
void menuSelector( Bitmap *menu_back, Sprite *selector);

/**
 * @brief Calls the proper fuction according to the current option of the corresponding menu
 *
 * According to the menu in wich the user is, it calls the function corresponding to the current option selected (given by the corresponding menu enumeration)
 */
void call_func_menu();
/**@}*/

#endif //_MENUS_H_
