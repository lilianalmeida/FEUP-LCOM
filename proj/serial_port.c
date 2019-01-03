#include <lcom/lcf.h>
#include <lcom/timer.h>

#include "serial_port_macros.h"
#include "serial_port.h"
#include "initGame.h"
#include "keyboard.h"
#include "i8254.h"
#include "i8042.h"
#include "mouse.h"
#include "video_gr.h"

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
  }else if ((iir_check & RLS) == RLS && (read_LSR(&lsr_check) != 0)){
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
      printf("Transmited: %c\n", (char) byte);
      return 0;
    }
  }
  return 1;
}

int clean_RBR(){
  uint32_t temp;
  uint32_t iir_check;
  read_IIR(&iir_check);
  if(iir_check & RD){
    read_RBR(&temp);
  }
  return 0;
}

int waitingPlayer2(){
  int ipc_status;
  message msg;
  uint32_t r;
  uint8_t nbyte = 0; //scancode's number of bytes
  bool wait = false;

  uint32_t timer_irq_set = getTIMER_IRQ();
  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();
  uint32_t uart_irq_set = getUART_IRQ();

  uint32_t charReceived = 'e'; //char random


  Bitmap* menu_back = loadBitmap("/home/lcom/labs/proj/bmp/waitingPlayer2.bmp");

  drawBitmap(menu_back,0, 0, ALIGN_LEFT);
  doubleBuffCall();
  clean_RBR();

  while (scanByte != ESC_CODE && charReceived != '1') {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & timer_irq_set) {
          if((counter_t % 2) ==0){
            clean_RBR();
            write_THR('2');

            //printf("sent\n");
          }
        }
        if (msg.m_notify.interrupts & kbc_irq_set) {
          kbc_asm_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & mouse_irq_set) {
          OB_cleaner();
        }
        if(msg.m_notify.interrupts & uart_irq_set){
          serialPort_handler(&charReceived);


        }
        break;
        default:
        break;
      }
    }
  }
  deleteBitmap(menu_back);
  if(scanByte == ESC_CODE){
    printf("Exited with esc\n" );
    return 1;
  }

  return 0;
}
int waitingPlayer1(){
  int ipc_status;
  message msg;
  uint32_t r;
  uint8_t nbyte = 0; //scancode's number of bytes
  bool wait = false;

  uint32_t timer_irq_set = getTIMER_IRQ();

  uint32_t kbc_irq_set = getKBC_IRQ();
  uint32_t mouse_irq_set = getMOUSE_IRQ();
  uint32_t uart_irq_set = getUART_IRQ();

  uint32_t charReceived = 'e'; //char random

  Bitmap* menu_back = loadBitmap("/home/lcom/labs/proj/bmp/waitingPlayer1.bmp");

  drawBitmap(menu_back,0, 0, ALIGN_LEFT);
  doubleBuffCall();
  clean_RBR();
  
  while (scanByte != ESC_CODE && charReceived != '2') {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
        if (msg.m_notify.interrupts & timer_irq_set) {
          if((counter_t % 2) ==0){
            clean_RBR();
            write_THR('1');
          }
        }
        if (msg.m_notify.interrupts & kbc_irq_set) {
          kbc_asm_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }
          isTwoByte(&wait, &nbyte);

          tickdelay(micros_to_ticks(DELAY_US));
        }
        if (msg.m_notify.interrupts & mouse_irq_set) {
          OB_cleaner();
        }
        if(msg.m_notify.interrupts & uart_irq_set){
          serialPort_handler(&charReceived);

        }
        break;
        default:
        break;
      }
    } else {
    }
  }

  deleteBitmap(menu_back);

  if(scanByte == ESC_CODE){
    printf("Exited with esc\n" );
    return 1;
  }

  return 0;
}
