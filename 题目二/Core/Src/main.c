/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h" //使用了PA6，没有使用PA2
#include "usart.h"//使用了PA6，没有使用PA2
#include "gpio.h" //使用了PA6，没有使用PA2

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DHT11_PIN      GPIO_PIN_6
#define DHT11_PORT     GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void delay_us(uint32_t us);
uint8_t DHT11_Start(void);
uint8_t DHT_Read_Byte(void);
void DHT_Read(void);
//先声明一下用到的函数，免的后面因为顺序原因又报错

//这个地方用于延时，相比参考文档里面的，他那个只能适配c8t6,这个系统定时器更好用一点
void delay_us(uint32_t us)
{
    uint32_t ticks = us * 72;
    uint32_t start = SysTick->VAL;
    while ((start - SysTick->VAL) < ticks);
}


uint8_t DHT11_Start(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 这里我们根据gpio.c里面的格式主动把PA6配置成推挽输出模式，（因为后面要切换成上拉模式），用来给DHT11发起始低电平，一开始看文档配置成开漏输出了，这里再写一遍
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);                // 查询资料显示不少于18，这里用20ms相当于预留时间
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    delay_us(30);                

    // 这里切换上拉，如果不用上拉，那么stm32会被锁在高电平，HD111无法进行操作
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);

    //等待拉低，如果没有拉低的话可以预防卡死
    uint32_t timeout = 0;
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100)
    {
        delay_us(1);
        timeout++;
    }
    if (timeout >= 100) return 0;

    
    timeout = 0;
    while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100)
    {
        delay_us(1);
        timeout++;
    }
    if (timeout >= 100) return 0;

    return 1;
}


uint8_t DHT_Read_Byte(void)//根据参考文档
{
    uint8_t dat = 0;
    uint32_t timeout = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        
        timeout = 0;
        while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100)
        {
            delay_us(1);
            timeout++;
        }
        if (timeout >= 100) return 0xFF;

       
        timeout = 0;
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && timeout < 100)
        {
            delay_us(1);
            timeout++;
        }
        if (timeout >= 100) return 0xFF;

        dat <<= 1;
        if (timeout > 40)
        {
            dat |= 0x01;
        }
    }
    return dat;
}


void DHT_Read(void)//传感器
{
    uint8_t buf[5] = {0};

    if (!DHT11_Start())
    {
        printf("传感器无响应\r\n");//野火小智学到的重定向，重定向在下面
        HAL_Delay(2000);
        return;
    }

    //
    for (uint8_t i = 0; i < 5; i++)
    {
        buf[i] = DHT_Read_Byte();
        if (buf[i] == 0xFF)
        {
            printf("读取的数据错误\r\n");
            HAL_Delay(2000);
            return;
        }
    }

    //参考文档，如果前四个和不等于校验和的话，就提醒校验失败，并完成最后的打印
    if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
    {
        float hum = buf[0] + buf[1] / 10.0f;
        float temp = buf[2] + buf[3] / 10.0f;
        printf("湿度:%.1f %%  温度:%.1f ℃\r\n", hum, temp);
    }
    else
    {
        printf("校验失败\r\n");
    }
    HAL_Delay(2000);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    DHT_Read();//主函数里面就放一个读取就可以
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

//这里是从野火小智学到的重定向，方便使用，参考文档里面也有
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);//最后也可以写成最大延迟时间HAL_MAX_DELAY
    return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/