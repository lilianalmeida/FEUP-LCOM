#ifndef _RTC_MACROS_H_
#define _RTC_MACROS_H_

/** @defgroup RTC_macros RTC_macros
 * @{
 *
 * Constants for programming the RTC
 */

#define BIT(n) 		(0x01 << (n))

#define RTC_IRQ					8		/**< @brief RTC IRQ line */

#define RTC_ADDR_REG			0x70	/**< @brief RTC’s internal register*/
#define RTC_DATA_REG			0x71 	/**< @brief RTC’s internal register*/

#define MIN_ADDRESS				0x02	/**< @brief Address of the minutes*/
#define HOUR_ADDRESS			0x04	/**< @brief Address of the hour*/
#define DAY_ADDRESS				0x07 	/**< @brief Address of the day*/
#define MONTH_ADDRESS			0x08	/**< @brief Address of the month*/
#define YEAR_ADDRESS			0x09	/**< @brief Address of the year*/

#define RTC_REG_A				0x0A	/**< @brief Address of the register A*/
#define RTC_REG_B				0x0B	/**< @brief Address of the register B*/
#define RTC_REG_C				0x0C	/**< @brief Address of the register C*/

#define SET_REG_B				0x07 	/**< @brief Command to configure register B*/
#define RTC_UIE					BIT(4)	/**< @brief Update interrupt pending*/

/**@}*/

#endif /* _RTC_MACROS_H_ */
