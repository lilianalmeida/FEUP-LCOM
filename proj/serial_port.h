#ifndef _SERIAL_PORT_
#define _SERIAL_PORT_

int serialPort_handler(uint32_t *scancode);
int serialPort_subscribe(uint8_t * bit_no);
int serialPort_unsubscribe();
int read_RBR(uint32_t *byte);
int read_LSR(uint32_t * lsr_byte);
int read_IIR(uint32_t * iir_check);
int write_THR(uint32_t byte);

#endif
