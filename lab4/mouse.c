#include <lcom/lcf.h>
#include "mouse.h"
#include "macros.h"


int (mouse_subscribe)(int * bit_no) {

  if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,bit_no) != 0){
    printf("Error subscribing mouse\n");
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe)(int * mouse_id) {

  int erro = sys_irqrmpolicy(mouse_id);
    if (erro != OK) {
      printf("Error in sys_irqrmpolicy", 0);
      return erro;
    }
    return 0;
  }

  int (mouse_enable)(){
    int counter =0;
    while(counter <5){ //Return true if the mouse was enabled succesfully in less than 5 tries
      if(write_kbc(KBC_CMD_INIT)==0 && write_kbc(MOUSE_ENABLE)== 0)
      return 0;
      counter++;
      tickdelay(micros_to_ticks(DELAY_US));
    }
    return 1;
  }


  int (write_kbc)(uint32_t cmd_byte){

    int cnt = 0;
    uint32_t verificationBits;
    while(cnt < 5){ //Loops 5 times before exiting without succes
      sys_outb(KBC_CMD_REG, KBC_CMD_INIT); //prepares mouse for writing
      sys_outb(IN_BUFF,cmd_byte); //writes the command byte
      tickdelay(micros_to_ticks(DELAY_US));
      sys_inb(OUT_BUFF,&verificationBits);

      if (verificationBits != NACK || verificationBits != ERROR){
        break;//if there are no erros breaks the loop and returns 0
      }
      tickdelay(micros_to_ticks(DELAY_US));
      cnt++;
    }

    if(cnt == 5){ //returns 1 if the previous loop failed 5 times
      return 1;
    }

    return 0;
  }

  int (OB_cleaner)(){

    uint32_t trash, status;

    if(sys_inb(STAT_REG,&status) != 0) //reads the current status of the output buffer
      return 1;

    while(status & OBF) //while the output buffer is full sends that date to the trash variable to clean the OB
      sys_inb(OUT_BUFF,&trash);

    return 0;
  }
