#ifndef _RTC_H_
#define _RTC_H_

int rtc_subscribe(uint8_t * bit_no);
int rtc_unsubscribe();

uint32_t get_minutes_rtc();
uint32_t get_hour_rtc();
uint32_t get_day_rtc();
uint32_t get_month_rtc();
uint32_t get_year_rtc();

#endif


