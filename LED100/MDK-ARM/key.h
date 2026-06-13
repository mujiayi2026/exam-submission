#ifndef _KEY_H_
#define _KEY_H_

#include "bsp.h"

#define K1 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)

void KeyInit(void);
void KeyScan(void);

#endif
