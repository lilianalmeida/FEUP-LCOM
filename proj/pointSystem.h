#ifndef _POINT_SYSTEM_H_
#define _POINT_SYSTEM_H_
#include "bitmap.h"

void loadGameNumbers();
void loadHighScoreNumbers();
void deleteNumbers();
bool pointHandler(Sprite* ball, Sprite* aim);
void printPoints();
void update_date ();
void print_date ();
void print_numbers (int digit, int32_t pos_x, int32_t pos_y);
void highscoreScreen();
void readHighscores();
void setHighscores(int score);
void BCD_to_binary(uint32_t * number);

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

typedef struct {
  uint32_t day;
  uint32_t month;
  uint32_t year;
  uint32_t hour;
  uint32_t minutes;
} Time_RTC;

typedef struct{
  int points;
  char date[8];
}HighscoreLine;

#endif
