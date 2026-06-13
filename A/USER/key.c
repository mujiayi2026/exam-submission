#include "key.h"

unsigned char k0,k0l;

void Key(void)
{
	k0=K0;
	if( k0l & !k0 )
	{
			HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
	}
	k0l=K0;
}

 