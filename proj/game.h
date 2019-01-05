#ifndef _GAME_H_
#define _GAME_H_
/** @defgroup Game Game
* @{
* Functions for manipulating single and multiplayer game
*/

/**
* @brief Manipulates single player game
*
* Draws necessary bitmaps and sprites
* Changes the player position as the keyboard keys are pressed
* Changes the aim of the mouse position upon mouse interrupts
* At every interrupt of the timer the position of the player and the ball are updated in the screen as well as the date and the score
* At every interrupt of the rtc the date and the hour are updated in the struct
* Exits upon release of the ESC key
*/
void singlePlayerGame();

/**
* @brief Manipulates player 1 of the multiplayer game
*
* Guarantees that both players are in sync and than holds all the game logic and function calls
* At every interrupt of the timer the position of the player and the ball are updated in the screen as well as the date and the score
* At every interrupt of the rtc the date and the hour are updated in the struct
* Exits upon release of the ESC key or when one player wins
*/
void gamePlayer1();

/**
* @brief Manipulates player 2 of the multiplayer game
*
* Guarantees that both players are in sync and than holds all the game logic and function calls
* At every interrupt of the timer the position of the player and the ball are updated in the screen as well as the date and the score
* At every interrupt of the rtc the date and the hour are updated in the struct
* Exits upon release of the ESC key or when one player wins
*
* Exits upon release of the ESC key
*/
void gamePlayer2();
/**@}*/

#endif
