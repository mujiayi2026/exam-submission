#ifndef _SR04_H_
#define _SR04_H_
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include <stdio.h>

#define TH HAL_GPIO_WritePin (GPIOA,GPIO_PIN_1,GPIO_PIN_SET)//PA1为trig引脚，置一为高电平，置零为低电平
#define TL HAL_GPIO_WritePin (GPIOA,GPIO_PIN_1,GPIO_PIN_RESET)
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
float SR04_GetDistance(void);
void SR04_Init(void);
#endif
