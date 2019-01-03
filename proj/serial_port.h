#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

/** @defgroup Serial_Port Serial_Port
* @{
* Functions for using the serial port
*/

/**
 * @brief Subscribes and enables serial port interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int serialPort_subscribe(uint8_t * bit_no);

/**
 * @brief Unsubscribes serial port interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int serialPort_unsubscribe();

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param scancode
 * @return
 */
int serialPort_handler(uint32_t *scancode);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param byte
 * @return
 */
int read_RBR(uint32_t *byte);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param lsr_byte
 * @return
 */
int read_LSR(uint32_t * lsr_byte);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param iir_check
 * @return
 */
int read_IIR(uint32_t * iir_check);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @param byte
 * @return
 */
int write_THR(uint32_t byte);

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @return
 */
int clean_RBR();

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @return
 */
int waitingPlayer2();

/**
 * @brief NEEDS TO BE DONE!!!!!!
 *
 * @return
 */
int waitingPlayer1();
/**@}*/

#endif //_SERIAL_PORT_
