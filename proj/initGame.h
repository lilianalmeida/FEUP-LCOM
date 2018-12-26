#ifndef _LCOM_INIT_GAME_H_
#define _LCOM_INIT_GAME_H_

uint32_t getKBC_IRQ();
uint32_t getTIMER_IRQ();
uint32_t getMOUSE_IRQ();

uint32_t getRTC_IRQ() ;
int rtc_subscribe(uint8_t * bit_no);
int rtc_unsubscribe();


int devices_init ();
int devices_end ();


#endif
