#ifndef _LCOM_INIT_GAME_H_
#define _LCOM_INIT_GAME_H_

uint32_t getKBC_IRQ();
uint32_t getTIMER_IRQ();
uint32_t getMOUSE_IRQ();

int devices_init ();
int devices_end ();


#endif
