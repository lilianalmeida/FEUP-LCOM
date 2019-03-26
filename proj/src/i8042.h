#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard and mouse
 */

#define BIT(n) 		(1 << (n))

#define KEYBOARD_IRQ 			1			/**< @brief Keyboard IRQ line */

#define STAT_REG 				0x64		/**< @brief Status Register*/
#define KBC_CMD_REG          	0x64 		/**< @brief Keyboard command register*/
#define OUT_BUFF 				0x60		/**< @brief Output Buffer*/
#define IN_BUFF              	0x60 		/**< @brief Input Buffer*/

#define TWO_BYTE_SCANCODE 		0xE0 		/**< @brief First byte of a two byte scancode*/

#define PAR_ERR 				BIT(7)		/**< @brief Parity Error*/
#define TO_ERR 					BIT(6)		/**< @brief Timeout Error*/
#define OBF 					BIT(0)		/**< @brief Output buffer full*/
#define IBF 				  	BIT(1)		/**< @brief Input buffer full*/

#define ESC_CODE 				0x81		/**< @brief Break code of esc key*/
#define KEY_W           		0x11		/**< @brief Make code of W key*/
#define KEY_A          			0x1E		/**< @brief Make code of A key*/
#define KEY_S          			0x1F		/**< @brief Make code of S key*/
#define KEY_D           		0x20		/**< @brief Make code of D key*/
#define KEY_ENTER       		0x9C		/**< @brief Break code of enter key*/
#define KEY_W_BREAK     		0x91		/**< @brief Break code of W key*/
#define KEY_A_BREAK     		0x9E		/**< @brief Break code of A key*/
#define KEY_S_BREAK     		0x9F		/**< @brief Break code of S key*/
#define KEY_D_BREAK     		0xa0		/**< @brief Break code of D key*/

#define KEY_UP          		0x48		/**< @brief Make code of up arrow key*/
#define KEY_LEFT        		0x4B		/**< @brief Make code of left arrow key*/
#define KEY_DOWN        		0x50		/**< @brief Make code of down arrow key*/
#define KEY_RIGHT       		0x4D		/**< @brief Make code of right arrow key*/
#define KEY_UP_BREAK    		0xC8		/**< @brief Break code of up arrow key*/
#define KEY_LEFT_BREAK  		0xCB		/**< @brief Break code of left arrow key*/
#define KEY_DOWN_BREAK  		0xD0		/**< @brief Break code of down arrow key*/
#define KEY_RIGHT_BREAK 		0xCD		/**< @brief Break code of right arrow key*/

#define DELAY_US 				20000


/* MOUSE RELATED MACROS */
#define MOUSE_IRQ           	12		/**< @brief Mouse IRQ line */

#define KBC_CMD_INIT         	0xD4 	/**< @brief Indicates to write byte to mouse*/
#define MOUSE_ENABLE		    0XF4 	/**< @brief Enable data reporting in stream only*/
#define MOUSE_DISABLE	        0XF5 	/**< @brief Disable data reporting in stream only*/
#define SET_STREAM				0xEA 	/**< @brief Set stream mode*/

#define MOUSE_BIT3            	BIT(3) 	/**< @brief The 3rd bit of the first byte*/
#define ACK					    0xFA 	/**< @brief Everything is OK*/
#define NACK					0xFE  	/**< @brief Invalid byte*/
#define ERROR	                0xFC  	/**< @brief Second consecutive invalid byte*/
/**@}*/

#endif/* _LCOM_I8042_H_ */
