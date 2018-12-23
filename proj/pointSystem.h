#ifndef _POINT_SYSTEM_H
#define _POINT_SYSTEM_H
#include "bitmap.h"

void loadNumbers();
bool pointHandler(Sprite* ball, Sprite* aim);
void printPoints();

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

} Numbers;

#endif
