#include <lcom/lcf.h>
#include "serial_port_macros.h"
#include "serial_port.h"

static int hook_id_uart = 5;

int serialPort_subscribe(uint8_t * bit_no) {

  *bit_no =(uint8_t) hook_id_uart;

  if (sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_uart) != OK) {
    return 1;
  }

  if (sys_outb(IER, RD_ENABLE|RLS_ENABLE) != 0) {
    printf("Error enabling interruptions to RD and RLS \n");
    return 1;
  }
  return 0;
}

int serialPort_unsubscribe() {

  if (sys_irqdisable(&hook_id_uart) != 0) {
    printf("Error: Failed to disable an interruption to the serial port\n\n");
    return 1;
  }
  if (sys_irqrmpolicy(&hook_id_uart) != OK) {
    return 1;
  }
  return 0;
}

int serialPort_handler(uint32_t *scancode){

  uint32_t iir_check;
  uint32_t lsr_check;

  if(read_IIR(&iir_check) != 0){
    return 1;
  }
  if((iir_check & RD) && (read_RBR(scancode) != 0)){
    return 1;
  }else if ((iir_check & RLS) == RLS&&(read_LSR(&lsr_check) != 0)){
    return 1;
  }
  return 0;
}

int read_RBR(uint32_t *byte){
  uint32_t lsr_check;

  uint8_t i = 0;

  while(i < 5){
    if(read_LSR(&lsr_check) != 0){
      printf("Error reading LSR\n");
    }
    else if(lsr_check & RECEIVER_DATA){
      sys_inb(RBR, byte);
      printf("Received: %c \n",(char) *byte); //debug
      tickdelay(micros_to_ticks(10000));
      return 0;
    }
    i++;
  }
  return 1;
}

int read_LSR(uint32_t * lsr_byte){
  if(sys_inb(LSR,lsr_byte) != 0){
    printf("Error: Failed to read Line Status Register data\n");
    return 1;
  }
  return 0;
}

int read_IIR(uint32_t * iir_check){
  if(sys_inb(IIR, iir_check) != 0){
    printf("Error reading Interrupt Identification Register data\n");
    return 1;
  }
  return 0;
}

int write_THR(uint32_t byte){
  uint32_t lsr_check;

  uint8_t i = 0;

  while(i < 5){
    if(read_LSR(&lsr_check) != 0){
      printf("Error reading LSR\n");
    } else if(lsr_check & THR_EMPTY){
      sys_outb(THR,byte);
      printf("Transmited:\n");
      return 0;
    }
  }
  return 1;
}
