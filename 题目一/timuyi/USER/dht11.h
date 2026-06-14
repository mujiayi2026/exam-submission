
#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f1xx_hal.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN  GPIO_PIN_2

typedef struct{
  uint8_t humi_int;
  uint8_t humi_deci;
  uint8_t temp_int;
  uint8_t temp_deci;
  uint8_t check_sum;
}DHT11_Data_TypeDef;

uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data);
#endif
