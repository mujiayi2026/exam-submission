#include "SR04.H"
extern TIM_HandleTypeDef  htim2;

/* 内部静态变量 */
static volatile uint32_t ic_val1 = 0;   //上升沿捕获值
static volatile uint32_t ic_val2 = 0;  //下降沿捕获值
static volatile uint8_t  capture_flag = 0;   // 状态0:等上升沿, 1:等下降沿, 2:完成

//微秒级延时函数（互联网搜索）用于sr04模块，触发微秒，开始测距
void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}
void SR04_Init(void)
{
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
	
}

float SR04_GetDistance(void)
{
    // 1.trig引脚触发脉冲
    TH;
    delay_us(15);
    TL;

    // 2. 等待捕获完成，超时约 30ms
    uint32_t timeout = 30000;
    while (capture_flag != 2 && timeout--);

    if (capture_flag == 2)
    {
        // 3. 计算高电平时间（μs）
        uint32_t pulse;
        if (ic_val2 > ic_val1)
            pulse = ic_val2 - ic_val1;//时间间隔
        else
            pulse = (0xFFFF - ic_val1) + ic_val2 + 1;   // 溢出回绕处理

        float distance = pulse / 58.0f;   // cm

        capture_flag = 0;   // 复位标志
        return distance;
    }
		//超时部分是ai的
    else
    {
        // 超时：复位输入捕获
        HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
        __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
        capture_flag = 0;
        return -1.0f;
    }
}

/* 输入捕获中断回调 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)//定时器二通道一
    {
        if (capture_flag == 0)   // 等待上升沿
        {
            ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//读取初始时间值
            capture_flag = 1;//等待下降沿
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else if (capture_flag == 1)   // 下降沿
        {
            ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);//下降沿到达时间值
            capture_flag = 2;//完成捕获
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}



	
	

 