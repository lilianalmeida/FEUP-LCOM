#include <machine/asm.h>

#include "rtc_macros.h"

.intel_syntax
.global _C_LABEL(rtc_ih_asm)

.global _C_LABEL(get_day_asm)
.global _C_LABEL(get_month_asm)
.global _C_LABEL(get_year_asm)
.global _C_LABEL(get_hour_asm)
.global _C_LABEL(get_minutes_asm)


.text

_C_LABEL(rtc_ih_asm):
  xor eax, eax            #resets eax to 0
  mov al, RTC_REG_C       #moves to al the address of register C that needs to be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG     #reads the content in register C
  ret

_C_LABEL(get_day_asm):
	xor eax, eax			      #resets eax to 0
  mov al, DAY_ADDRESS     #moves to al the address of the register where the day can be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG	    #reads the day	
  ret

_C_LABEL(get_month_asm):
	xor eax, eax		        #resets eax to 0
  mov al, MONTH_ADDRESS   #moves to al the address of the register where the month can be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG	    #reads the month	
  ret

_C_LABEL(get_year_asm):
	xor eax, eax			      #resets eax to 0    
  mov al, YEAR_ADDRESS    #moves to al the address of the register where the year can be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG     #reads the year
  ret

_C_LABEL(get_hour_asm):
  xor eax, eax            #resets eax to 0
  mov al, HOUR_ADDRESS    #moves to al the address of the register where the hour can be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG     #reads the hour
  ret

_C_LABEL(get_minutes_asm):
  xor eax, eax            #resets eax to 0
  mov al, MIN_ADDRESS     #moves to al the address of the register where the minutes can be read
  out RTC_ADDR_REG, al    #writes to RTC_ADDR register what RTC register to be accessed
  in al, RTC_DATA_REG     #reads the minutes
  ret


