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

	//通用GPIO 初始化
	bsp_InOutInit();
	
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
	
	//按键初始化
	 KEY_Init();
	 //IO初始化

	//串口初始化
	bsp_InitUart(COM1,9600);
	bsp_InitUart(COM2,9600);

	/* 初始化gui */
/*
  UG_Init(&gui,void(*)(UG_S16,UG_S16,UG_COLOR))_HW_DrawPoint,240,320);
	UG_DriverRegister(DRIVER_DRAW_LINE,(void*)_HW_DrawLine);
	UG_DriverRegister(DRIVER_FILE_FRAME,(void*)_HW_FillLine);
	UG_DriverEnable(DRIVER_DRAW_LINE);
	UG_DricerEnable(DRIVER_FILL_FRAME);
*/

	//spi初始化
	SPI_Flash_Init();

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

uint32_t bsp_GetRCCofGPIO(GPIO_TypeDef* GPIOx)
{
	uint32_t rcc;

	if (GPIOx == GPIOA)
	{
		rcc = RCC_APB2Periph_GPIOA;
	}
	else if (GPIOx == GPIOB)
	{
		rcc = RCC_APB2Periph_GPIOB;
	}
	else if (GPIOx == GPIOC)
	{
		rcc = RCC_APB2Periph_GPIOC;
	}
	else if (GPIOx == GPIOD)
	{
		rcc = RCC_APB2Periph_GPIOD;
	}
	else if (GPIOx == GPIOE)
	{
		rcc = RCC_APB2Periph_GPIOE;
	}
	else if (GPIOx == GPIOF)
	{
		rcc = RCC_APB2Periph_GPIOF;
	}
	else if (GPIOx == GPIOG)
	{
		rcc = RCC_APB2Periph_GPIOG;
	}
	return rcc;
}


/* --------------------------------------end of file--------------------------------------- */
