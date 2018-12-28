#ifndef _SERIAL_MACROS_
#define _SERIAL_MACROS_

#define BIT(n) 		(0x01 << (n))

#define COM1_IRQ				4

//Adresses
#define RBR                  0x3F8
#define THR                  0x3F8
#define IER                  0x3F9
#define IIR                  0x3FA
#define LSR                  0x3FD

//IER
#define RD_ENABLE            BIT(0)
#define THR_ENABLE           BIT(1)
#define RLS_ENABLE           BIT(2)

//LSR
#define RECEIVER_DATA        BIT(0)
#define OVERRUN_ERROR        BIT(1)
#define PARITY_ERROR         BIT(2)
#define FRAMING_ERROR        BIT(3)
#define THR_EMPTY            BIT(5)

//IIR
#define RLS                  (BIT(1)|BIT(2))
#define RD                   BIT(2)
#define IIR_THR              BIT(1)


#endif
