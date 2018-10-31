#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#define BIT(n) (0x01 << (n))

#define KEYBOARD_IRQ         	1
#define MOUSE_IRQ           	12

#define STAT_REG             	0x64 	// Status Register
#define CTRL_REG             	0x64 	// Control Register
#define KBC_CMD_REG          	0x64 	// Keyboard command register
#define KBC_CMD_INIT         	0xd4 	// Write byte to mouse
#define OUT_BUFF             	0x60 	// Output Buffer
#define IN_BUFF              	0x60 	// Input Buffer
#define ESC_BREAK            	0x81	// Breakcode of Escape key
#define MSB                  	0x80 	// Most significant bit
#define TWO_BYTE_CODE	        0xe0  // Double byte scancode

#define OBF                   BIT(0)  // Output buffer full
#define ACK					        	0xFA 	// Everything is OK
#define NACK					        0xFE  // Invalid byte
#define ERROR	                0xFC  // Second consecutive invalid byte
#define MOUSE_ENABLE		    	0XF4 	// Enable data reporting in stream only
#define MOUSE_DISABLE	        0XF5  // Disable data reporting in stream only

#define DELAY_US 20000

#endif
