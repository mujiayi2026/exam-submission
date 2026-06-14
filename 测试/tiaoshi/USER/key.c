

#include "key.h"

static unsigned char k0, k0_last;

void Key_scan(void)
{
    k0 = KEY0;
   
    if(k0_last != k0 && k0 == 0)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }
    k0_last = k0;
}
/*void Key_scan(void)
{
    
    if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0)
    {
        HAL_Delay(20);  
        
       
        if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); 
            
           
            while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0);
        }
    }
}*/

