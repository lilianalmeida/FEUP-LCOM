#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n) 		(0x01 << (n))

#define STAT_REG 				0x64
#define KBC_CMD_REG 			0x64
#define OUT_BUF 				0x60
#define INTR_ENABLE 			BIT(0)
#define INTR_DISABLE 			0xef

#define TWO_BYTE_SCANCODE 		0xE0

#define KEYBOARD_IRQ 			1

#define READ_COMMAND			0x20
#define WRITE_COMMAND 			0x60

#define PAR_ERR 				BIT(7)
#define TO_ERR 					BIT(6)
#define IBF 					BIT(1)
#define OBF 					BIT(0)
#define AUX						BIT(5)

#define ESC_CODE 				0x81
#define KEY_W           		0x11
#define KEY_A          			0x1E
#define KEY_S          			0x1F
#define KEY_D           		0x20
#define KEY_ENTER       		0x9C
#define KEY_W_BREAK     		0x91
#define KEY_A_BREAK     		0x9E
#define KEY_S_BREAK     		0x9F
#define KEY_D_BREAK     		0xa0

#define KEY_UP          		0x48
#define KEY_LEFT        		0x4B
#define KEY_DOWN        		0x50
#define KEY_RIGTH       		0x4D
#define KEY_UP_BREAK    		0xC8
#define KEY_LEFT_BREAK  		0xCB
#define KEY_DOWN_BREAK  		0xD0
#define KEY_RIGTH_BREAK 		0xCD



#define DELAY_US 				20000

#endif
