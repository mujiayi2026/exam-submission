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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define DHT11_GPIO_PORT   GPIOA
#define DHT11_GPIO_PIN    GPIO_PIN_2
//微秒级延时（防止超时）
static void delay_us(uint32_t us)
{
    uint32_t i;
    for (i = 0; i < us * 8; i++)
    {
        __NOP();//用NOP消耗时间，让延时函数真的延时，防止被编译器略过
    }
}
//毫秒级延时
static void delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

//设置输出（初始化引脚，输出模式，上下拉，和速度）
static void DHT11_SetOutput(void)
{
    GPIO_InitTypeDef gpio_init = {0};//初始化
    gpio_init.Pin = DHT11_GPIO_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
    gpio_init.Pull = GPIO_PULLUP;//上拉模式
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_init);
}

//设置输入
static void DHT11_SetInput(void)
{
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = DHT11_GPIO_PIN;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;//上拉输入
    HAL_GPIO_Init(DHT11_GPIO_PORT, &gpio_init);
}

//写低电平（亮）
static void DHT11_WriteLow(void)
{
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
}

//写高电平（灭）
static void DHT11_WriteHigh(void)
{
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
}

//读取引脚的状态
static uint8_t DHT11_ReadPin(void)
{
    return HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

//起始信号，开始读取DHT11
static void DHT11_Start(void)
{
    DHT11_SetOutput();
    DHT11_WriteLow();
    delay_ms(20);
    DHT11_WriteHigh();
    delay_us(30);
    DHT11_SetInput();
}

static uint8_t DHT11_CheckResponse(void)
{
    uint8_t retry = 0;
    while (DHT11_ReadPin() && retry < 100)
    {
        delay_us(1);
        retry++;
    }
    if (retry >= 100) return 1;
    
    retry = 0;
    while (!DHT11_ReadPin() && retry < 100)
    {
        delay_us(1);
        retry++;
    }
    if (retry >= 100) return 1;
    return 0;
}

//等待高电平开始
// 读1位
static uint8_t DHT11_ReadBit(void)
{
    uint8_t retry = 0;
    while ( !DHT11_ReadPin() && retry < 100 )
    {
        delay_us(1);
        retry++;
    }
    delay_us(40);
    if ( DHT11_ReadPin() )
        return 1;
    else
        return 0;
}

// 读1字节
static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= DHT11_ReadBit();
    }
    return data;
}

// 读取温湿度数据
uint8_t DHT11_ReadData(uint8_t *humidity, uint8_t *temperature)
{
    uint8_t i;
    uint8_t buf[5];
    
    DHT11_Start();
    if (DHT11_CheckResponse() != 0) return 1;
    
    for (i = 0; i < 5; i++)
    {
        buf[i] = DHT11_ReadByte();
    }
    
//校验（返回值0=成功，返回值2=失败）
    if (buf[0] + buf[1] + buf[2] + buf[3] != buf[4]) 
	return 2;
    *humidity = buf[0];
    *temperature = buf[2];
    return 0;
}

// printf 重定向到串口
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);
    return ch;
}
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
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  //串口打印
	printf("DHT11 Test Start\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

uint8_t humi, temp;
	  //读取温湿度（0表示成功，否则读取失败）
if (DHT11_ReadData(&humi, &temp) == 0)
{
    printf("湿度: %d%%  温度: %d°C\r\n", humi, temp);
}
else
{
    printf("Read Error\r\n");
}
HAL_Delay(2000);
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

/* USER CODE BEGIN 4 */

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
