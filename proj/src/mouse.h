#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "bitmap.h"

extern unsigned int byteNumber; //number of bytes already read from a packet
extern bool mouse_ih_error; //true if there's an error in the mouse handler, false otherwise

/** @defgroup Mouse Mouse
* @{
* Functions for using the mouse
*/

/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe(uint8_t * bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();

/**
 * @brief Enables stream mode data reporting, by sending the respective command to the mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_enable_data();

/**
 * @brief Disables stream mode data reporting, by sending the respective command to the mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable_data();

/**
 * @brief Sends a command directly to the mouse
 *
 * Writes KBC-command 0xD4 to allow to send a command to the mouse, sends that command and reads the acknowledgment byte sent by the KBC
 * @param cmd_byte command to send to the mouse
 * @return Return 0 upon success and non-zero otherwise
 */
int write_kbc(uint32_t cmd_byte);

/**
 * @brief Parses the packet received
 *
 * Parses the three bytes of a packet into a packey struct
 */
void parse_packet();

/**
 * @brief Set stream mode, by sending the respective command to the mouse.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int set_stream_mode();

/**
 * @brief Reads a byte of a pakcet if there's one to read and discards it
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int OB_cleaner();

/**
 * @brief Changes the position of the aim of the mouse according to the packet's displacements in x and y
 */
void move_aim (Sprite *aim);

/**
 * @brief Returns whether the left button of the mouse is pressed or not
 *
 * @return Return true if left button is pressed, false otherwise
 */
bool is_left_pressed();
/**@}*/

#endif /*_MOUSE_H_*/
