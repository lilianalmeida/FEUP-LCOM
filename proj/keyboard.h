#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

//global variables
extern bool kbc_ih_error; //true if there's an error in the keyboard handler, false otherwise
extern uint32_t scanByte; //scanbyte read from keyboard

/** @defgroup Keyboard Keyboard
* @{
* Functions for using the keyboard
*/

/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_subscribe(uint8_t * bit_no);


/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe();

/**
 * @brief Checks if the last scancode read has one or two bytes and if it needs to wait for another cycle to finish reading a two_byte scancode
 *
 * @param wait true if a two byte scancode, false otherwise
 * @param nbyte number of bytes of the scancode read
 */
void isTwoByte(bool *wait, uint8_t *nbyte);
/**@}*/

#endif
