#ifndef _LCOM_MACRO_H_
#define _LCOM_MACRO_H_

#define BIT(n) (0x01 << (n))

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF 0x60
#define INTR_ENABLE BIT(0)

#define TWO_BYTE_SCANCODE 0xE0

#define KEYBOARD_IRQ 1

#define READ_COMMAND 0x20
#define WRITE_COMMAND 0x60
//#define ARG_COMMAND 0x60

#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)
#define IBF BIT(1)
#define OBF BIT(0)
#define AUX BIT(5)

#define ESC_CODE 0x81

#define DELAY_US 20000

 #ifdef LAB3
 int sys_inb_count(port_t port, uint32_t *byte);
 #else
 #define sys_inb_cnt(p,q) sys_inb(p,q)
 #endif

#endif /* _LCOM_MACRO_H */
