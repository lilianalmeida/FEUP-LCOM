#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t UNUSED(timer), uint32_t UNUSED(freq)) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *UNUSED(bit_no)) {
  /* To be completed by the students */


  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
	/* To be completed by the students */
	if (timer > 2 || timer < 0)
		return -1;

	char readBack = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

	int returnValue = sys_outb(TIMER_CTRL, readBack);
	if (returnValue != 0) {
		printf("Error in sys_outb");
		return returnValue;
	}

	unsigned long stLong;
	returnValue = sys_inb(TIMER_0 + timer, &stLong);
	if (returnValue != 0) {
		printf("Error in sys_inb");
		return returnValue;
	}

	*st = stLong;
	return 0;

}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
 
	timer_status_field_val uni = {};
	timer_print_config(timer, field,uni);
	
	/* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
