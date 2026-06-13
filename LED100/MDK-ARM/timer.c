#include "timer.h"

void TimerInit(void)
{
    MX_TIM6_Init();
    HAL_TIM_Base_Start_IT(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim6)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
    }
}
