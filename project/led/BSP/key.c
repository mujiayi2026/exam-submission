#include "key.h"

uint8_t Key1_Scan(void)
{
  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
  {
    HAL_Delay(20); // Ïû¶¶
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
    {
      while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0); // µÈ´ýËÉ¿ª
      return 1;
    }
  }
  return 0;
}