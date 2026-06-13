#include "bsp.h"

void SysInit()
{
	LedInit();
	KeyInit();
	TimerInit();
	//UartInit();
}


void SysRun()
{
	KeyScan();
	//uart_proc();
}
