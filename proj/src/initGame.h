#ifndef _LCOM_INIT_GAME_H_
#define _LCOM_INIT_GAME_H_

/** @defgroup InitGame InitGame
* @{
* Subscribers and unsubscribers of all I/O devices
*/

/**
 * @brief Returns the bit activated by the hook_id that subscribed the keyboard interrupts 
 * 
 * @return keyboard irq_set
 */
uint32_t getKBC_IRQ();

/**
 * @brief Returns the bit activated by the hook_id that subscribed the timer interrupts 
 * 
 * @return timer irq_set
 */
uint32_t getTIMER_IRQ();

/**
 * @brief Returns the bit activated by the hook_id that subscribed the mouse interrupts 
 * 
 * @return mouse irq_set
 */
uint32_t getMOUSE_IRQ();

/**
 * @brief Returns the bit activated by the hook_id that subscribed the serial port interrupts 
 * 
 * @return serial port irq_set
 */
uint32_t getUART_IRQ();

/**
 * @brief Returns the bit activated by the hook_id that subscribed the rtc interrupts 
 * 
 * @return rtc irq_set
 */
uint32_t getRTC_IRQ() ;

/**
 * @brief Subscribes and enables interrupts of all I/O devices and initializes the video module in graphics mode
 * 
 * Subscribes timer 0, keyboard, mouse, rtc e serial port and intializes the video module in mode 117
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int devices_init ();

/**
 * @brief Unsubscribes all I/O devices interrupts and returns the video module to default
 *
 * Unsubscribes timer 0, keyboard, mouse, rtc e serial port and returns the video module to default Minix 3 text mode
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int devices_end ();
/**@}*/

#endif
