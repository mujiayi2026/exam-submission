#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"
#include "tim.h"
#include "TIMER.h"
void DHT11_out(void);
uint8_t  DHT11_read_byte();

uint8_t  DHT11_use(uint8_t *temp,uint8_t *hum);


extern uint8_t buf[5];
#endif

