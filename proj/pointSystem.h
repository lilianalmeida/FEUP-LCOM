#ifndef _POINT_SYSTEM_H_
#define _POINT_SYSTEM_H_
#include "bitmap.h"

/** @defgroup PointSystem PointSystem
* @{
* Functions for manipulating the score and the date
*/

///Struct to save the bitmaps of the numbers for the score and the date
typedef struct {
  Bitmap* No0;
  Bitmap* No1;
  Bitmap* No2;
  Bitmap* No3;
  Bitmap* No4;
  Bitmap* No5;
  Bitmap* No6;
  Bitmap* No7;
  Bitmap* No8;
  Bitmap* No9;
  Bitmap* NoDash;
  Bitmap* NoSlash;
} Numbers;

///Struct to save the current date
typedef struct {
  uint32_t day;
  uint32_t month;
  uint32_t year;
  uint32_t hour;
  uint32_t minutes;
} Time_RTC;

///Struct to save the points and date of each player saved in the highscore text file
typedef struct{
  int points;
  char date[8];
}HighscoreLine;


/**
 * @brief Loads the bmp images of the numbers to use in the score and the date in the game and puts them in a Numbers struct
 */
void loadGameNumbers();

/**
 * @brief Loads the bmp images of the numbers to use in the score and the date in the hishscores and puts them in a Numbers struct
 */
void loadHighScoreNumbers();

/**
 * @brief Deletes the bitmaps in a Numbers struct
 */
void deleteNumbers();

/**
 * @brief Increases the score if the aim of the mouse was pointing inside the field
 *
 * If the aim of the mouse when the ball collided with the player's racket was inside the field (except the uppermost and lower rectangles)
 * the score increases one point, otherwise the game ends and the score is saved
 * @param aim sprite of the aim of the mouse
 * @param xLeft Left boudarie of the aim position
 * @param xRight Right boudarie of the aim position
 * @return Return true if the score increased, false otherwise
 */
 bool pointHandler(Sprite *aim, int xLeft, int xRight);

/**
 * @brief Prints the score
 * @param isMulti Set to true if the game mode is multiplayer
 */
 void printPoints(bool isMulti, int score1, int score2);

/**
 * @brief Calls rtc handler in assembly and updates the date and time using RTC
 */
void update_date ();

/**
 * @brief Prints the date and time
 */
void print_date ();

/**
 * @brief Draws the digit given in the posision (x,y) received with the bitmaps saved in a Numbers struct
 *
 * @param digit digit to be drawn
 * @param pos_x position x of the digit
 * @param pos_y position y of the digit
 */
void print_numbers (int digit, int32_t pos_x, int32_t pos_y);

/**
 * @brief Converts a BCD number to binary number
 *
 * @param number number to be converted
 */
void BCD_to_binary(uint32_t * number);

/**
 * @brief Draws the highscores saved in the highscores text file and exits when esc key is pressed, discarding all mouse packets read
 */
void highscoreScreen();

/**
 * @brief Saves in the highscores text file the score of the player (ordered by score), if it's better than the other ones already saved
 *
 * @param score final score of the player
 */
void setHighscores(int score);

/**
 * @brief Reads the highscores from the highscores text file and prints them in the screen
 */
void readHighscores();
/**@}*/

#endif
