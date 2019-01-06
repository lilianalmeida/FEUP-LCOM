#include <lcom/lcf.h>
#include "bitmap.h"
#include "video_gr.h"


Bitmap* loadBitmap(const char* filename) {
  // allocating necessary size
  Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

  // open filename in read binary mode
  FILE *filePtr;
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
  return NULL;

  // read the bitmap file header
  BitmapFileHeader bitmapFileHeader;
  fread(&bitmapFileHeader, 2, 1, filePtr);

  // verify that this is a bmp file by check bitmap id
  if (bitmapFileHeader.type != 0x4D42) {
    fclose(filePtr);
    return NULL;
  }

  int rd;
  do {
    if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
    break;
    if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
    break;
    if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
    break;
  } while (0);

  if (rd != 1) {
    fprintf(stderr, "Error reading file\n");
    exit(-1);
  }

  // read the bitmap info header
  BitmapInfoHeader bitmapInfoHeader;
  fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

  // move file pointer to the begining of bitmap data
  fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

  // allocate enough memory for the bitmap image data
  unsigned char* bitmapImage = (unsigned char*) malloc(
    bitmapInfoHeader.imageSize);

    // verify memory allocation
    if (!bitmapImage) {
      free(bitmapImage);
      fclose(filePtr);
      return NULL;
    }

    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);

    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
      fclose(filePtr);
      return NULL;
    }

    // close file and return bitmap image data
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;

    return bmp;
  }

  void drawBitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
    return;

    int width = bmp->bitmapInfoHeader.width;
    int drawWidth = width;
    int height = bmp->bitmapInfoHeader.height;

    if (alignment == ALIGN_CENTER)
    x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
    x -= width;

    if (x + width < 0 || x > getHorResolution() || y + height < 0
    || y > getVerResolution())
    return;

    int xCorrection = 0;
    if (x < 0) {
      xCorrection = -x;
      drawWidth -= xCorrection;
      x = 0;

      if (drawWidth > getHorResolution())
      drawWidth = getHorResolution();
    } else if (x + drawWidth >= getHorResolution()) {
      drawWidth = getHorResolution() - x;
    }

    char* bufferStartPos;
    char* imgStartPos;

    int i;
    for (i = 0; i < height; i++) {
      int pos = y + height - 1 - i;

      if (pos < 0 || pos >= getVerResolution())
      continue;

      bufferStartPos = (char*) getGraphicsBuffer();
      bufferStartPos += x * 2 + pos * getHorResolution() * 2;

      imgStartPos =(char*)( bmp->bitmapData) + xCorrection * 2 + i * width * 2;

      memcpy(bufferStartPos, imgStartPos, drawWidth * 2);


    }
  }

  void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
    return;

    free(bmp->bitmapData);
    free(bmp);
  }

  Sprite* createSprite(Bitmap* bmp, uint32_t x, uint32_t y, uint32_t xspeed, uint32_t yspeed){
    Sprite* sp = (Sprite*)malloc(sizeof(Sprite));
    sp->bmp = bmp;
    sp->x = x;
    sp->y = y;
    sp->xspeed = xspeed;
    sp->yspeed = yspeed;
    sp->height = bmp->bitmapInfoHeader.height;
    sp->width = bmp->bitmapInfoHeader.width;
    sp->align = ALIGN_LEFT;
    sp->colided = false;
    sp->canColide = true;
    sp->mov.MOVE_UP = false;
    sp->mov.MOVE_LEFT = false;
    sp->mov.MOVE_RIGHT = false;
    sp->mov.MOVE_DOWN = false;
    return sp;
  }

  void drawSprite( Sprite* sp){
    if (sp->bmp == NULL)
    return;

    int width = sp->width;
    int drawWidth = width;
    int height = sp->bmp->bitmapInfoHeader.height;
    uint32_t x = sp->x;
    uint32_t y = sp->y;

    if (drawWidth > getHorResolution())
    drawWidth = getHorResolution();
    else if (x + drawWidth >= (uint32_t)getHorResolution()) {
      drawWidth = getHorResolution() - x;
    }

    unsigned char* bufferStartPos;
    unsigned char* imgStartPos;

    int i;
    sp->colided = false;

    for (i = 0; i < height; i++) {
      int pos = y + height - 1 - i;

      if (pos < 0 || pos >= getVerResolution())
      continue;

      bufferStartPos = getGraphicsBuffer();
      bufferStartPos += x * 2 + pos * getHorResolution() * 2;

      imgStartPos =(unsigned char*)( sp->bmp->bitmapData) + i * width * 2;

      int j;
      for(j = 0; j < drawWidth * 2; j += 2){
        if((imgStartPos[j]!=0x1F && imgStartPos[j+1] != 0xF8) && (bufferStartPos[j]==0x00 && bufferStartPos[j+1] == 0x80))
        {
          if(sp->canColide && !sp->colided){
            sp->colided = true;
          }

        }

        if(imgStartPos[j]!=0x1F && imgStartPos[j+1] != 0xF8){
          bufferStartPos[j] = imgStartPos[j];
          bufferStartPos[j+1] = imgStartPos[j+1];
        }
      }
    }

    if(sp->x > (getHorResolution()/2) && !sp->canColide){
      sp->canColide = true;
    }

  }
