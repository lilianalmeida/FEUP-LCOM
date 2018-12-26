#include <lcom/lcf.h>
#include <stdint.h>
#include "rtc_macros.h"


//global variable
static int hook_id_rtc = 0x04;
static int hook_id_uart = 0x05;

int rtc_subscribe(uint8_t * bit_no) {

  	uint32_t temp;

  	if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) return -1;

  	sys_inb(RTC_DATA_REG, &temp);

  	temp = (temp & 0x27);
  	if (sys_outb(RTC_ADDR_REG, RTC_REG_B) != OK) return -1;
  	if (sys_outb(RTC_DATA_REG, temp) != OK) return -1;

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


/*void convert_hour(uint32_t *hour){
  uint32_t temp;
  temp = *hour;
  printf("%x\n", *hour);

  if (temp < 13){
    return;
  }
  temp = *hour - 0x80;
  temp += 12;
  if (temp == 12){
    *hour = 0;
  }

  *hour = temp;
}*/

uint32_t get_minutes_rtc(){
  uint32_t min;
  sys_outb(RTC_ADDR_REG, 0x02);
  sys_inb(RTC_DATA_REG, &min);
  return min;
}
uint32_t  get_hour_rtc(){
  uint32_t min;
  sys_outb(RTC_ADDR_REG, 0x04);
  sys_inb(RTC_DATA_REG, &min);
  return min;
}
uint32_t get_day_rtc(){
  uint32_t min;
  sys_outb(RTC_ADDR_REG, 0x07);
  sys_inb(RTC_DATA_REG, &min);
  return min;
}

uint32_t get_month_rtc(){
  uint32_t min;
  sys_outb(RTC_ADDR_REG, 0x08);
  sys_inb(RTC_DATA_REG, &min);
  return min;
}

uint32_t get_year_rtc(){
  uint32_t min;
  sys_outb(RTC_ADDR_REG, 0x09);
  sys_inb(RTC_DATA_REG, &min);
  return min;
}



int serialPort_subscribe(uint8_t * bit_no) {

	*bit_no = hook_id_uart;

	if (sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_uart) != OK) {
		return 1;
	}
	return 0;
}

int serialPort_unsubscribe() {
	if (sys_irqrmpolicy(&hook_id_uart) != OK) {
		return 1;
	}
	return 0;
}



