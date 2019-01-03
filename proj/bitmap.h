#ifndef _BITMAP_H_
#define _BITMAP_H_

/** @defgroup Bitmap Bitmap
* @{
* Functions for manipulating bitmaps
*/

typedef enum {
  ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {

  unsigned short type;  /**< @brief specifies the file type*/
  unsigned int size;  /**< @brief specifies the size in bytes of the bitmap file*/
  unsigned int reserved; /**< @brief must be 0*/
  unsigned int offset; /**< @brief specifies the offset in bytes from the bitmapfileheader to the bitmap bits*/
} BitmapFileHeader;

typedef struct {
  unsigned int size; /**< @brief specifies the number of bytes required by the struct*/
  int width; /**< @brief specifies width in pixels*/
  int height; /**< @brief specifies height in pixels */
  unsigned short planes; /**< @brief specifies the number of color planes, must be 1*/
  unsigned short bits; /**< @brief specifies the number of bit per pixel*/
  unsigned int compression; /**< @brief specifies the type of compression*/
  unsigned int imageSize; /**< @brief size of image in bytes*/
  int xResolution; /**< @brief number of pixels per meter in x axis*/
  int yResolution; /**< @brief number of pixels per meter in y axis*/
  unsigned int nColors; /**< @brief number of colors used by the bitmap*/
  unsigned int importantColors; /**< @brief number of colors that are important*/
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
  BitmapInfoHeader bitmapInfoHeader;
  unsigned char* bitmapData;
} Bitmap;

/// Struct to save in which directions is a Sprite moving
typedef struct{
  bool MOVE_UP,MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT;
} Movement;

///Represents a Sprite
typedef struct{
  int32_t x; /**< @brief specifies the position x in pixels*/
  int32_t y; /**< @brief specifies the position x in pixels*/
  uint32_t xspeed; /**< @brief specifies the horizontal speed of the sprite (negative if the sprite moves from right to left, positive otherwise)*/
  uint32_t yspeed; /**< @brief specifies the vertical speed of the sprite (negative if the sprite moves upwards, positive otherwise)*/
  Bitmap *bmp; /**< @brief specifies the bitmap corresponding to the sprite*/
  uint32_t height; /**< @brief specifies height in pixels*/
  uint32_t width; /**< @brief specifies width in pixels*/
  Alignment align; /**< @brief specifies the image alignment*/
  bool colided; /**< @brief true if the sprite collided with the color of the player's racket, false otherwise*/
  bool canColide; /**< @brief true if the sprite can collide with the color of the the player's racket, false otherwise*/
  Movement mov; /**< @brief specifies the directions of the movement*/
} Sprite;


/**
* @brief Loads a bmp image
*
* @param filename Path of the image to load
* @return Non NULL pointer to the image buffer
*/
Bitmap* loadBitmap(const char* filename);

/**
* @brief Draws an unscaled, unrotated bitmap at the given position
*
* @param bitmap bitmap to be drawn
* @param x destiny x coord
* @param y destiny y coord
* @param alignment image alignment
*/
void drawBitmap(Bitmap* bitmap, int x, int y, Alignment alignment);

/**
* @brief Destroys the given bitmap, freeing all resources used by it.
*
* @param bmp bitmap to be destroyed
*/
void deleteBitmap(Bitmap* bmp);

/**
* @brief Creates a sprite with the given bitmap, with a position (x,y) and speed in each direction
*
* @param bmp bitmap of the sprite
* @param x destiny x coord
* @param y destiny y coord
* @param xspeed speed of the sprite in the x axis
* @param yspeed speed of the sprite in the y axis
* @return Non NULL pointer to the sprite
*/
Sprite* createSprite(Bitmap* bmp, uint32_t x, uint32_t y, uint32_t xspeed, uint32_t yspeed);

/**
* @brief Draws an unscaled, unrotated Sprite at the position especified in the sprite struct 
*
* @param sp sprite to be drawn
*/
void drawSprite( Sprite* sp);
/**@}*/

#endif //_BITMAP_H_

