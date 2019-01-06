#ifndef _RTC_H_
#define _RTC_H_

/** @defgroup RTC RTC
* @{
* Functions for using the RTC
*/

/**
 * @brief Subscribes and enables RTC interrupts and configures resgister B
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_subscribe(uint8_t * bit_no);

/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe();

/**
 * @brief Reads what is in Register C, using RTC's internal registers 
 *
 * Developed in assembly, reads Register C to clear any pending interrupt
 */
void rtc_ih_asm();

/**
 * @brief Reads the day of the current date, using RTC's internal registers 
 *
 * Developed in assembly
 * @return Return day
 */
uint32_t get_day_asm();

/**
 * @brief Reads the month of the current date, using RTC's internal registers 
 *
 * Developed in assembly
 * @return Return month
 */
uint32_t get_month_asm();

/**
 * @brief Reads the year of the current date, using RTC's internal registers 
 *
 * Developed in assembly
 * @return Return year
 */
uint32_t get_year_asm();

/**
 * @brief Reads the hour of current hours, using RTC's internal registers 
 *
 * Developed in assembly
 * @return Return hour
 */
uint32_t get_hour_asm();

/**
 * @brief Reads minutes of current hours, using RTC's internal registers 
 *
 * Developed in assembly
 * @return Return minutes
 */
uint32_t get_minutes_asm();
/**@}*/

#endif


