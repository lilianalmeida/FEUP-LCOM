#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n) 		(0x01 << (n))

#define STAT_REG 					0x64
#define KBC_CMD_REG 				0x64
#define OUT_BUF 					0x60
#define INTR_ENABLE 				BIT(0)
#define INTR_DISABLE 				0xef

#define TWO_BYTE_SCANCODE 			0xE0

#define KEYBOARD_IRQ 				1

#define READ_COMMAND				0x20
#define WRITE_COMMAND 				0x60

#define PAR_ERR 					BIT(7)
#define TO_ERR 						BIT(6)
#define IBF 						BIT(1)
#define OBF 						BIT(0)
#define AUX							BIT(5)

#define ESC_CODE 				0x81
#define KEY_W           0x0011
#define KEY_A           0x001E
#define KEY_S           0x001F
#define KEY_D           0x0020
#define KEY_ENTER       0x001C

#define DELAY_US 					20000

#endif
