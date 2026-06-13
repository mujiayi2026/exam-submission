#include "key.h"

unsigned int keytick = 0;
bool k1l;
bool k1;

void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void KeyScan(void)
{
    if(HAL_GetTick() - keytick < 20) return;
    else
    {
        keytick = HAL_GetTick();
        k1 = K1;
        if(!k1 && k1l)
        {
            HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
        }
        k1l = k1;
    }
}
