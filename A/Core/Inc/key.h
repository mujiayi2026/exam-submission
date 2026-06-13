#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"
#include "gpio.h"


#define K0 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)

void Key(void);

#endif