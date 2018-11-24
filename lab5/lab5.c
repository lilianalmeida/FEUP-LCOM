// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "macro.h"
#include "video_gr.h"
#include "keyboard.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {

	if (vg_init(mode) == NULL){
		printf("Error:failed to set graphics mode\n");
		return 1;
	}

	tickdelay (micros_to_ticks (delay*1000000));
	
	if (vg_exit() != OK){
		printf("Error:failed to set default Minix 3 text mode\n");
		return 1;
	}


  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  	uint8_t bit_no;
	int ipc_status;
	message msg;
	uint32_t r;


	if(keyboard_subscribe(&bit_no) != OK){
		printf("Error enabling keyboard interrupts\n");
		return 1;
	}
	uint32_t irq_set = BIT(bit_no);

	if(vg_init(mode) == NULL){
		printf("Error setting graphics mode\n");
		return 1;
	}

	if (vg_draw_rectangle (x,y,width,height,color) != OK){
		printf("Error drawing rectangle\n");
		return 1;
	}

	while (scanByte != ESC_CODE) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

				 	kbc_ih();

					if (kbc_ih_error) {
            			kbc_ih_error = false;
            			continue;
					}

					tickdelay(micros_to_ticks(DELAY_US));
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error disabling keyboard interrupts\n");
		return 1;
	}

	if (vg_exit() != OK){
		printf("Error:failed to set default Minix 3 text mode\n");
		return 1;
	}
	printf("Set to text mode\n");
	return 0;

}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
	uint8_t bit_no;
	int ipc_status;
	message msg;
	uint32_t r;


	if(keyboard_subscribe(&bit_no) != OK){
		printf("Error enabling keyboard interrupts",0);
		return 1;
	}
	uint32_t irq_set = BIT(bit_no);

	if(vg_init(mode) == NULL){
    vg_exit();
		printf("Error setting graphics mode\n");
		return 1;
	}

	if (drawPattern(no_rectangles, first, step) != OK){
		printf("Error drawing pattern\n");
		return 1;
	}


	while (scanByte != ESC_CODE) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

				 	kbc_ih();

					if (kbc_ih_error) {
            			kbc_ih_error = false;
            			continue;
					}

					tickdelay(micros_to_ticks(DELAY_US));
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error disabling keyboard interrupts\n");
		return 1;
	}

	if (vg_exit() != OK){
		printf("Error:failed to set default Minix 3 text mode\n");
		return 1;
	}
	printf("Set to text mode\n");
	return 0;
}


int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y){

 	uint8_t bit_no;
	int ipc_status;
	message msg;
	uint32_t r;


	if(keyboard_subscribe(&bit_no) != OK){
		printf("Error enabling keyboard interrupts",0);
		return 1;
	}
	uint32_t irq_set = BIT(bit_no);

	if(vg_init(MODE105) == NULL){
		printf("Error setting graphics mode\n");
		return 1;
	}

	if (drawSprite (xpm, x, y) != OK){
		printf("Error drawing sprite\n");
		return 1;
	}

	while (scanByte != ESC_CODE) {
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */

				 	kbc_ih();

					if (kbc_ih_error) {
            			kbc_ih_error = false;
            			continue;
					}

					tickdelay(micros_to_ticks(DELAY_US));
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (keyboard_unsubscribe() != 0) {
		printf("Error disabling keyboard interrupts\n");
		return 1;
	}

	vg_exit();
	printf("Set to text mode\n");
	return 0;
 }
 
int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate){

  uint8_t bit_no_kbd, bit_no_timer;
  int ipc_status, j = 60/fr_rate;
  message msg;

  Sprite *sprite;

  if (j <= 0) 
  	return 1;

  if (xf == xi) {
    sprite = create_sprite(xpm, xi, yi, 0, speed);
  }
  else if (yf == yi) {
    sprite = create_sprite(xpm, xi, yi, speed, 0);
  } else {
    printf("Invalid movement", 0);
    return 1;
  }

  int r;
  if ((r = keyboard_subscribe(&bit_no_kbd)) != OK) {
    printf("Error in keyboard_subscribe", 0);
    return r;
  }

  if ((r = timer_subscribe_int(&bit_no_timer)) != OK) {
    printf("Error in timer_subscribe_int", 0);
    return r;
  }

  uint32_t irq_set_kbd = BIT(bit_no_kbd);
  uint32_t irq_set_timer = BIT(bit_no_timer);

  if(vg_init(MODE105) == NULL){
    printf("Error setting graphics mode\n");
    return 1;
  }

  uint32_t counter_t =0;
  while (scanByte != ESC_CODE) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set_timer) {/* subscribed interrupt */
          timer_int_handler();
          if (counter_t % j == 0) {
          	move_sprite(sprite, xi, yi, xf, yf, speed);
          }
    	}	
        if (msg.m_notify.interrupts & irq_set_kbd) { /* subscribed interrupt */

          kbc_ih();

          if (kbc_ih_error) {
            kbc_ih_error = false;
            continue;
          }

          tickdelay(micros_to_ticks(DELAY_US));
        }
        break;
        default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

   destroy_sprite(sprite);

  if (keyboard_unsubscribe() != 0) {
    printf("Error disabling keyboard interrupts\n");
    return 1;
  }

  if (timer_unsubscribe_int()!= OK) {
    printf("Error in timer_unsbscribe_int", 0);
    return 1;
  }
  vg_exit();
  printf("Set to text mode\n");
  return 0;
}


 int (video_test_controller)(){
 	if (vbe_get_controller_info() != OK){
 		printf("Error gettinf the VBE controller information \n");
 		return 1;
 	}
   return 0;
 }
