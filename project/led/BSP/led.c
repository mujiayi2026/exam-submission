#include "led.h"

void LED1_Toggle(void)
{
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}

void LED2_Toggle(void)
{
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7);
}