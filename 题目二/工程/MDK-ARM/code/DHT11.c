#include "DHT11.h"
//MCU通过总线开启DHT11，使用gpioa的0引脚
void DHT11_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
//DHT11应答MCU
void DHT11_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
//DHT11读取单个字节
uint8_t  DHT11_read_byte()
{
	uint8_t dat=0;
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==RESET);
		HAL_Delay_Us(40);
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==SET)
		{
			dat|=(1 << (7 - i)); 
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==SET);
		}
	}
	
	return dat;
}	
uint8_t buf[5]={0};//装数据
//DHT读取数据
uint8_t  DHT11_use(uint8_t *temp,uint8_t *hum)
{
	uint8_t i=0;
		DHT11_out();
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
	HAL_Delay(20);//根据DHT11的通信协议进行延时
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
		HAL_Delay_Us(35);
		DHT11_in();
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==RESET)
		{	
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==RESET);
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==SET);
			for(int i=0;i<5;i++)
			{
				buf[i]=DHT11_read_byte();
			}
			if(buf[0]+buf[1]+buf[2]+buf[3]==buf[4])//校验成功
			{
				*temp=buf[2];
				*hum=buf[0];
				return 0;
			}
		}
	return 1;
}