#include"judgement.h"
#include <string.h>
#include "usart.h"
//用if else判断光照强度
char*Get_light_level(uint16_t adc_ori)
{
  static char light_level[20];
	if(adc_ori <=1000) 
			strcpy(light_level,"strong");
		else if (adc_ori <=2200)
			strcpy(light_level,"normAL");
		else if (adc_ori <=3200)
				strcpy(light_level,"WEAK");
		else 
			strcpy(light_level,"dark");
		return light_level;
}