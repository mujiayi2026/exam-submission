#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "stm32f1xx_hal.h"


#define KEY0 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)

void Key_scan(void);

#endif