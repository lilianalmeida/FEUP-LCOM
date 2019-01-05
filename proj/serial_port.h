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
 * @brief Handles the chars received if there are any
 *
 * @param scancode holds the char received
 * @return Return 0 upon success and non-zero otherwise
 */
int serialPort_handler(uint32_t *scancode);

/**
 * @brief Reads the Receiver Buffer Register
 *
 * @param byte gets changed according to what is received
 * @return Return 0 upon success and non-zero otherwise
 */
int read_RBR(uint32_t *byte);

/**
 * @brief Reads LSR
 *
 * @param lsr_byte  gets changed according to what is read
 * @return Return 0 upon success and non-zero otherwise
 */
int read_LSR(uint32_t * lsr_byte);

/**
 * @brief Reads the Interrupt Identification Register
 *
 * @param iir_check  gets changed according to what is read
 * @return Return 0 upon success and non-zero otherwise
 */
int read_IIR(uint32_t * iir_check);

/**
 * @brief Writes a char to be sent to the THR
 *
 * @param byte byte with the char info to send
 * @return Return 0 upon success and non-zero otherwise
 */
int write_THR(uint32_t byte);

/**
 * @brief Cleans the Receiver Buffer Register
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int clean_RBR();

/**
 * @brief Guarantees that both players are connected and in sync. One computer must call this function while the other call waitingPlayer1.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int waitingPlayer2();

/**
 * @brief Guarantees that both players are connected and in sync. One computer must call this function while the other call waitingPlayer2.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int waitingPlayer1();
/**@}*/

#endif //_SERIAL_PORT_
