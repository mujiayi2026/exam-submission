#include"hcsr04.h"
void HCSR04StartTrigStart(void)
{
 HCSR_TRIG_HIGH();
	HAL_Delay(20);
	HCSR_TRIG_LOW();
	
}
// 读取Echo高电平持续时间
uint32_t HCSR04_ReadEchoTime(void)
{
uint32_t us_cnt=0;
	while(HCSR_ECHO()==GPIO_PIN_RESET) ;
	while(HCSR_ECHO()==GPIO_PIN_SET) 
	{
	  us_cnt++;
		HAL_Delay(1);
	}
	return us_cnt;
}
//计算距离
float HCSR04_GetDistance(void)
{
    HCSR04StartTrigStart();
	  uint32_t t =0;
	uint32_t timeout=10000;
	while(HCSR_ECHO()==GPIO_PIN_RESET&&timeout--) ;
 if(timeout==0) return 0;
	timeout=40000;
	while(HCSR_ECHO()==GPIO_PIN_SET&&timeout--)
	{
		t++;
		HAL_Delay (1);
	}//调试后增加超时机制，防卡
	return t=(float )t/58.0F;
	
}	
//均值滤波，减小误差
float Distance (uint8_t cot)
{
   float sum=0;
	 for(int c =0;c <= cot;c++)
	 {
	 sum+=HCSR04_GetDistance ();
	 }
	 return sum/cot;
}