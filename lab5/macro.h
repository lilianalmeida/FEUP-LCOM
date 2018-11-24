#ifndef _LCOM_MACRO_H_
#define _LCOM_MACRO_H_

#define BIT(n) 		(0x01 << (n))
#define PB2BASE(x)	(((x) >> 4) &  0x0F000)
//#define PB2OFF(x)	((x) & 0x0FFFF)

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

#define ESC_CODE 					0x81
#define MODE105 					0x105
#define SET_VBE						0x4F02
#define LINEAR_FB					0x105
#define VBE_INT						0x10 /* BIOS video services */
#define VBE_INFO					0x4F00
#define VBE_MODE_INFO				0x4F01

#define BACKGROUNDCOLOR 0

#define DELAY_US 					20000
#define DELAY_TOS 					1000000

#endif /* _LCOM_MACRO_H */
