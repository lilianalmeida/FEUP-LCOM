#include <lcom/lcf.h>
#include "pointSystem.h"
#include "playerMovement.h"
#include "bitmap.h"
#include "video_gr.h"

static int32_t score = 0;
static Numbers numbers;
void loadNumbers(){
  Bitmap* No0 = loadBitmap("/home/lcom/labs/proj/bmp/No0.bmp");
  Bitmap* No1 = loadBitmap("/home/lcom/labs/proj/bmp/No1.bmp");
  Bitmap* No2 = loadBitmap("/home/lcom/labs/proj/bmp/No2.bmp");
  Bitmap* No3 = loadBitmap("/home/lcom/labs/proj/bmp/No3.bmp");
  Bitmap* No4 = loadBitmap("/home/lcom/labs/proj/bmp/No4.bmp");
  Bitmap* No5 = loadBitmap("/home/lcom/labs/proj/bmp/No5.bmp");
  Bitmap* No6 = loadBitmap("/home/lcom/labs/proj/bmp/No6.bmp");
  Bitmap* No7 = loadBitmap("/home/lcom/labs/proj/bmp/No7.bmp");
  Bitmap* No8 = loadBitmap("/home/lcom/labs/proj/bmp/No8.bmp");
  Bitmap* No9 = loadBitmap("/home/lcom/labs/proj/bmp/No9.bmp");

  numbers.No0 = No0;
  numbers.No1 = No1;
  numbers.No2 = No2;
  numbers.No3 = No3;
  numbers.No4 = No4;
  numbers.No5 = No5;
  numbers.No6 = No6;
  numbers.No7 = No7;
  numbers.No8 = No8;
  numbers.No9 = No9;
}

bool pointHandler(Sprite*UNUSED(ball), Sprite *aim){
  int aimx = getAimx();
  int aimy = getAimy();

  if(aimx > getHorResolution()/2 && aimx < 1010- (int)aim->width && aimy > 236 && aimy < 591 -(int)aim->height){
    score++;
    resetAim();
    return true;
  }
  else{
    score = 0;
    return false;
  }
}

void printPoints(){

  int units = score % 10;
  int dozens = (score/10) % 10;
  int hundreds = (score/100)%10;

  int32_t unitsX =110;
  int32_t unitsY = 42;
  int32_t dozensX =98;
  int32_t dozensY=42;
  int32_t hundredsX=86;
  int32_t hundredsY=42;

  switch(hundreds){
    case 0:
    drawBitmap(numbers.No0,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 1:
    drawBitmap(numbers.No1,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 2:
    drawBitmap(numbers.No2,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 3:
    drawBitmap(numbers.No3,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 4:
    drawBitmap(numbers.No4,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 5:
    drawBitmap(numbers.No5,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 6:
    drawBitmap(numbers.No6,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 7:
    drawBitmap(numbers.No7,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 8:
    drawBitmap(numbers.No8,hundredsX,hundredsY,ALIGN_LEFT);
    break;
    case 9:
    drawBitmap(numbers.No9,hundredsX,hundredsY,ALIGN_LEFT);
    break;
  }

  switch(dozens){
    case 0:
    drawBitmap(numbers.No0,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 1:
    drawBitmap(numbers.No1,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 2:
    drawBitmap(numbers.No2,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 3:
    drawBitmap(numbers.No3,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 4:
    drawBitmap(numbers.No4,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 5:
    drawBitmap(numbers.No5,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 6:
    drawBitmap(numbers.No6,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 7:
    drawBitmap(numbers.No7,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 8:
    drawBitmap(numbers.No8,dozensX,dozensY,ALIGN_LEFT);
    break;
    case 9:
    drawBitmap(numbers.No9,dozensX,dozensY,ALIGN_LEFT);
    break;
  }

  switch(units){
    case 0:
    drawBitmap(numbers.No0,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 1:
    drawBitmap(numbers.No1,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 2:
    drawBitmap(numbers.No2,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 3:
    drawBitmap(numbers.No3,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 4:
    drawBitmap(numbers.No4,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 5:
    drawBitmap(numbers.No5,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 6:
    drawBitmap(numbers.No6,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 7:
    drawBitmap(numbers.No7,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 8:
    drawBitmap(numbers.No8,unitsX,unitsY,ALIGN_LEFT);
    break;
    case 9:
    drawBitmap(numbers.No9,unitsX,unitsY,ALIGN_LEFT);
    break;
  }
}
