#ifndef _SPRITEMOV_H_
#define _SPRITEMOV_H_
#include "bitmap.h"

/** @defgroup SpriteMovement SpriteMovement
* @{
* Functions for manipulating players and ball movement
*/


/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param sp NEEDS TO BE DONE!!!!!!
 * @param nbyte NEEDS TO BE DONE!!!!!!
 * @param adversaryMov NEEDS TO BE DONE!!!!!!
 * @param isMulti NEEDS TO BE DONE!!!!!!
 */
void set_move(Sprite *sp, uint8_t nbyte, char adversaryMov, bool isMulti);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param sp sprite of the player
 * @param xLeft NEEDS TO BE DONE!!!!!!
 * @param xRight NEEDS TO BE DONE!!!!!!
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
 * @brief Gives the ball a random speed in x and y directions
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

void printTest(uint16_t t);//APAGAR

void remoteMoveBall(Sprite * ball, uint16_t movCode);


/**@}*/

#endif /* _SPRITEMOV_H_ */
