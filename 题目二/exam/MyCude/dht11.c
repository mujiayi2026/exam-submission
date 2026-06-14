#include "dht11.h"
#include "stdio.h"
#include "string.h"

static void Delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    uint32_t told = SysTick->VAL;
    uint32_t tnow = told;
    uint32_t tcnt = 0;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += told + (SysTick->LOAD - tnow);
            if (tcnt >= ticks)
                break;
            told = tnow;
        }
    }
}

static void DHT11_GPIO_Mode(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (mode == 0) 
    {
        GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);
    }
    else 
    {
        GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);
    }
}

void DHT11_Init(void)
{
    DHT11_GPIO_Mode(0); 
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET); 
    HAL_Delay(100);
}

uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i, j;

    DHT11_GPIO_Mode(0);
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(18); 
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
    Delay_us(30); 

    DHT11_GPIO_Mode(1);
    if (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET)
    {
        while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
        while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);

        for (i = 0; i < 5; i++)
        {
            uint8_t data = 0;
            for (j = 0; j < 8; j++)
            {
                while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_RESET);
                Delay_us(40);
                if (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET)
                {
                    data |= 1 << (7 - j);
                    while (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == GPIO_PIN_SET);
                }
            }
            buf[i] = data;
        }
    }
    return 0;  
}
