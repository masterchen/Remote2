#include "bsp.h"
//#include "bsp_ili9341_lcd.h"
#include "delay.h"
/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
  //SystemInit();   //系统初始化 72M
	
  //SysTick_init();	
	delay_init(72);
	/* Enable the CRC Module */
  /*CRC和emWin没有关系，只是他们为了库的保护而做的，
  这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。 */
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 
	/* LED 端口初始化 */ 
  //LED_GPIO_Config();
	LED_Init();// 这里有个Bug，不初始化LED 屏幕不亮，应该是IO配置移动在LCD初始化里边
	/*SramInit*/
	//FSMC_SRAM_Init();
	
	/* 初始化触屏 */
	//Touch_Init();
	
	/*屏初始化*/
	//bsp_InitLCD();
	TFTLCD_Init();
	//LCD_Init();		//LCD显示初始化	
	//LCD_Reginit();
	
	
	//串口初始化
	uart1_init(115200);
	/* 初始化gui */
	
}

/*
 * 函数名：SysTick_init
 * 描述  ：配置SysTick定时器
 * 输入  ：无
 * 输出  ：无
 */
void SysTick_init(void)
{
  /* 初始化并使能SysTick定时器 */
  SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ);
  
  /*  配置1ms 中断一次，即os的频率为1000hz */
	if (SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))	
	{ 
		/* Capture error */ 
		while (1);
	}
}


/* --------------------------------------end of file--------------------------------------- */
