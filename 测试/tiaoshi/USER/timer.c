#include "timer.h"
#include "key.h"

void TimInit(void)
{
    
    HAL_TIM_Base_Start_IT(&htim6);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
    }
}