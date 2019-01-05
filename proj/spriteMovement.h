#ifndef _SPRITEMOV_H_
#define _SPRITEMOV_H_
#include "bitmap.h"

/** @defgroup SpriteMovement SpriteMovement
* @{
* Functions for manipulating players and ball movement
*/


/**
* @brief Sets the current movement axis of a player
*
* @param sp Pointer to a player Sprite
* @param nbyte byte number for 2 byteScancodes. Makes arrow key movement possible
* @param adversaryMov char received from the other player that indicates its movement
* @param isMulti set to true if its multiplayer mode
*/
void set_move(Sprite *sp, uint8_t nbyte, char adversaryMov, bool isMulti);

/**
* @brief Moves player along the axis set with set_move()
*
* @param sp Pointer to a player Sprite
* @param xLeft left boundary of player movement
* @param xRight Right boundary of player movement
*/
void movePlayer(Sprite* sp, uint32_t xLeft, uint32_t xRight);

/**
* @brief Changes the speed of the ball when collided with the player's racket
*
* When the ball collides with the racket, the speed of the ball is changed in x and y so that it will end up where the aim of the mouse is in the exact moment of the collision
* @param ball sprite of the ball
* @param aim sprite of the aim associated with the mouse
*/
void shootBall(Sprite* ball, Sprite* aim);

/**
* @brief Returns the aim position in the x direction at the last shot in the ball
*
* @return Return the aim position in x
*/
int getAimx();

/**
* @brief Returns the aim position in the y direction at the last shot in the ball
*
* @return Return the aim position in y
*/
int getAimy();

/**
* @brief Resets the aim position
*
* Set the aim position (x,y) to zero
*/
void resetAim();

/**
* @brief Gives the ball a random speed in x and y directions.
*
* @param ball sprite of the ball
*/
void throwBall(Sprite* ball);

/**
* @brief Resets the ball speed control variable
*
* Set the ball speed control variable to the inicial value, 9
*/
void resetBallSpeed();

/**
* @brief Sets the ball speed according to what was received
*
* Decomposes a 16bit sequence into x and y speed values
*
* @param ball Pointer to a ball sprite
* @param moveCode 16bit sequence that represents the movement received
*
*/
void remoteMoveBall(Sprite * ball, uint16_t movCode);

/**@}*/

#endif /* _SPRITEMOV_H_ */
