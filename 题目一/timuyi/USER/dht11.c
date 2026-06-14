
#include "dht11.h"

uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t buf[5]={0};
    uint8_t i,j;


    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    HAL_Delay(1);


    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);


    for(i=0;i<5;i++)
    {
        uint8_t dat=0;
        for(j=0;j<8;j++)
        {
            while(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)==GPIO_PIN_RESET);
            HAL_Delay(1);
            if(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)==GPIO_PIN_SET)
            {
                dat |= 1<<(7-j);
                while(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)==GPIO_PIN_SET);
            }
        }
        buf[i]=dat;
    }

    DHT11_Data->humi_int = buf[0];
    DHT11_Data->temp_int = buf[2];

    return 0;
}

