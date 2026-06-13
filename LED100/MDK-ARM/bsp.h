#ifndef _BSP_H_
#define _BSP_H_

#include "main.h"
//#include "dma.h"
#include "timer.h"
//#include "usart.h"
#include "gpio.h"

#include "led.h"
#include "key.h"
#include "timer.h"
//#include "uart.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


void SysInit(void);
void SysRun(void);

#endif