#include <lcom/lcf.h>
#include <stdint.h>
#include "rtc_macros.h"

//global variable
static int hook_id_rtc = 0x04;

int rtc_subscribe(uint8_t * bit_no) {

  uint32_t temp;

  if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) 
    return 1;

  if (sys_inb(RTC_DATA_REG, &temp) != OK)
    return 1;

  temp = (temp & SET_REG_B) | RTC_UIE;
  
  if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) 
    return 1;
  if (sys_outb(RTC_DATA_REG, temp) != OK) 
    return 1;

	*bit_no = hook_id_rtc;

	if (sys_irqsetpolicy(RTC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_rtc) != OK) {
		return 1;
	}
	return 0;
}

int rtc_unsubscribe() {

	if (sys_irqrmpolicy(&hook_id_rtc) != OK) {
		return 1;
	}

	return 0;
}

