#ifndef _SERIAL_MACROS_H_
#define _SERIAL_MACROS_H_

/** @defgroup Serial_macros Serial_macros
 * @{
 *
 * Constants for programming the serial port
 */

#define BIT(n) 		(0x01 << (n))

#define COM1_IRQ				4			/**< @brief Serial Port IRQ line */

/* Adresses */
#define RBR                  0x3F8			/**< @brief Address of Receiver Buffer Register */
#define THR                  0x3F8			/**< @brief Address of Transmitter Holding Register */
#define IER                  0x3F9			/**< @brief Address of Interrupt Enable Register */
#define IIR                  0x3FA			/**< @brief Address of Interrupt Identification Register */
#define LSR                  0x3FD			/**< @brief Address of Line Status Register */

/* IER */
#define RD_ENABLE            BIT(0)			/**< @brief Enables the Received Data Available Interrupt */
//#define THR_ENABLE           BIT(1)			/**< @brief Enables the Transmitter Holding Register Empty Interrupt */
#define RLS_ENABLE           BIT(2)			/**< @brief Enables the Receiver Line Status Interrupt */

/* LSR */
#define RECEIVER_DATA        BIT(0)			/**< @brief Receiver Data */
//#define OVERRUN_ERROR        BIT(1)			/**< @brief Overrun Error */
//#define PARITY_ERROR         BIT(2)			/**< @brief Parity Error */
//#define FRAMING_ERROR        BIT(3)			/**< @brief Framing Error */
#define THR_EMPTY            BIT(5)			/**< @brief Transmitter Holding Register Empty */

/* IIR */
#define RLS                  (BIT(1)|BIT(2)) /**< @brief Receiver Line Status */
#define RD                   BIT(2)			/**< @brief Reived Data Available */
//#define IIR_THR              BIT(1) 		/**< @brief Transmitter Holding Register Empty */

/**@}*/
#endif /* _SERIAL_MACROS_H_ */
