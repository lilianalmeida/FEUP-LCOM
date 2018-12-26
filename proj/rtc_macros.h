#ifndef _RTC_MACROS_H_
#define _RTC_MACROS_H_

#define BIT(n) 		(0x01 << (n))

#define RTC_ADDR_REG			0x70
#define RTC_DATA_REG			0x71

#define MIN_ADDRESS				0x02
#define HOUR_ADDRESS			0x04
#define DAY_ADDRESS				0x07
#define MONTH_ADDRESS			0x08
#define YEAR_ADDRESS			0x09

#define RTC_REG_A				0x0A
#define RTC_REG_B				0X0B
#define RTC_REG_C				0X0C

#define RTC_IRQ					8
#define COM1_IRQ				4

#endif


