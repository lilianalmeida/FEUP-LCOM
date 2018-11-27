#include <lcom/lcf.h>
#include "bitmap.h"
#include <stdio.h>
#include "vbe_macros.h"


Bitmap* readBitmap(const char* file){


  FILE *filePointer;
  filePointer = fopen(file, "rb");
  if (filePointer == NULL)
  return NULL;

  BitmapFileHeader bitmapFileHeader;
  fread(&bitmapFileHeader.bfType, 2, 1, filePointer);

  if(bitmapFileHeader != BITMAPERROR){
    fclose(filePointer);
    return NULL;
  }

  if(fread(&bitmapFileHeader.bfSize, 4, 1, filePointer)!=1){
    fprintf(stderr, "Error reading file\n");
    exit(-1);
  }
  if(fread(&bitmapFileHeader.bfReserved, 4, 1, filePointer)!=1){
    fprintf(stderr, "Error reading file\n");
    exit(-1);
  }
  if(fread(&bitmapFileHeader.bfOffBits, 4, 1, filePointer)!=1){
    fprintf(stderr, "Error reading file\n");
    exit(-1);
  }

  BitmapInfoHeader bitmapInfoHeader;

  fread(&bitmapInfoHeader,sizeof(BitmapInfoHeader), 1,filePointer);

  fseek(filePointer,bitmapInfoHeader.bfOffBits, SEEK_SET);

  Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

  //alocar espaço necessário
  uint8_t* bitmapImage = (uint8_t*) malloc(sizeof(bitmapInfoHeader.imageSize));

  if (!bitmapImage) {
    free(bitmapImage);
    fclose(filePointer);
    return NULL;
  }

  fread(bitmapImage, sizeof(bitmapInfoHeader.imageSize),1,filePointer);

  if (bitmapImage == NULL) {
        fclose(filePointer);
        return NULL;
  }

  bmp->data = bitmapImage;
  bmp->bitmapInfoHeader = bitmapInfoHeader;

  return bmp;
}
