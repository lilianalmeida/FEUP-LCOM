#ifndef _BITMAP_H_
#define  _BITMAP_H_

typedef struct {
    uint16_t bfType; //file type
    uint32_t bfSize; //size in bytes of the bitmap file
    uint32_t bfReserved; // reserved; must be 0
    uint32_t bfOffBits; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
    uint32_t size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    uint16_t planes; // specifies the number of color planes, must be 1
    uint16_t bitCount; // specifies the number of bit per pixel
    uint32_t compression; // specifies the type of compression
    uint32_t imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    uint32_t nColors; // number of colors used by the bitmap
    uint32_t importantColors; // number of colors that are important
} BitmapInfoHeader;

typedef struct {
  BitmapInfoHeader bitmapInfoHeader;
  uint8_t* data;
} Bitmap;


Bitmap* readBitmap(const char* file);
void drawBitmap(Bitmap bm);
void eraseBitmap(Bitmap bm); //erases form screen
void deleteBitmap(Bitmap bm); //deletes from memory



#endif
