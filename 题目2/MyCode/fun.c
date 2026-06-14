#include "fun.h"

void function(void)
{
 OLED_show();
	//DHT数据采集
	
}

uint8_t OLED_buff[20];
uint32_t time_OLED;//在系统定时器中1ms自增

void OLED_show(void)
{
	if(time_OLED%200 <100) return ;//0.1秒刷新
	OLED_Clear();

	sprintf((char *)OLED_buff,"R_data[0]:%d",R_data[0]);
	OLED_ShowString(2,1,(char *)OLED_buff);

	
	sprintf((char *)OLED_buff,"R_data[2]:%d",R_data[2]);
	OLED_ShowString(4,1,(char *)OLED_buff);

}
