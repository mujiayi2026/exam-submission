#include "stm32f1xx_hal.h"

#ifndef __HCSR04_H
#define __HCSR04_H
//宏定义串口，PD0=Trig and PD1=Echo
#define HC_SR04_Trig_GPIO_Port    GPIOD
#define HC_SR04_Trig_Pin          GPIO_PIN_0
#define HC_SR04_ECHO_GPIO_Port    GPIOD
#define HC_SR04_ECHO_Pin          GPIO_PIN_1

// 
#define HCSR_TRIG_HIGH()  HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, GPIO_PIN_SET)
#define HCSR_TRIG_LOW()   HAL_GPIO_WritePin(HC_SR04_Trig_GPIO_Port, HC_SR04_Trig_Pin, GPIO_PIN_RESET)
#define HCSR_ECHO()       HAL_GPIO_ReadPin(HC_SR04_ECHO_GPIO_Port, HC_SR04_ECHO_Pin)

//函数声明
void HCSR04StartTrigStart(void);
uint32_t HCSR04_ReadEchoTime(void);
float HCSR04_GetDistance(void);

#endif
