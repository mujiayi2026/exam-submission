#include "t.h"
#include "led.h"
#include "stm32f1xx_hal.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
if(htim->Instance==TIM6)
{

  ledtoggle();
}

}

