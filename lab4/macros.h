#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#define BIT(n) 		(0x01 << (n))

#define KEYBOARD_IRQ         	1
#define MOUSE_IRQ           	12

#define STAT_REG             	0x64 	// Status Register
#define CTRL_REG             	0x64 	// Control Register
#define KBC_CMD_REG          	0x64 	// Keyboard command register
#define KBC_CMD_INIT         	0xD4 	// Write byte to mouse
#define OUT_BUFF             	0x60 	// Output Buffer
#define IN_BUFF              	0x60 	// Input Buffer
#define ESC_BREAK            	0x81	// Breakcode of Escape key
#define MSB                  	0x80 	// Most significant bit
#define TWO_BYTE_CODE	        0xE0  	// Double byte scancode
#define MOUSE_DIS_CMD			0xFD	// Mouse disable interrupts command
#define MOUSE_EN_CMD			0xFD	// Mouse enable interrupts command
#define READ_DATA				0xEB	// Send data packet request

#define READ_COMMAND 			0x20
#define WRITE_COMMAND 			0x60

#define IBF 				  	BIT(1)	// Input buffer full
#define OBF                   	BIT(0)  // Output buffer full
#define ACK					    0xFA 	// Everything is OK
#define NACK					0xFE  	// Invalid byte
#define ERROR	                0xFC  	// Second consecutive invalid byte
#define MOUSE_ENABLE		    0XF4 	// Enable data reporting in stream only
#define MOUSE_DISABLE	        0XF5 	// Disable data reporting in stream only
#define SET_REMOTE				0xF0 	// Set remote mode
#define SET_STREAM				0xEA 	// Set stream mode
#define MOUSE_BIT3            	BIT(3) 	// The 3rd bit of the first byte is always set



#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)


#define DELAY_US 				20000

#endif
