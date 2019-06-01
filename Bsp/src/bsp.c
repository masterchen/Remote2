#include "bsp.h"
//#include "bsp_ili9341_lcd.h"
#include "delay.h"
/*
 * ��������BSP_Init
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 * ����  ����
 * ���  ����
 */
void BSP_Init(void)
{
  //SystemInit();   //ϵͳ��ʼ�� 72M
	
  //SysTick_init();	
	delay_init(72);
	/* Enable the CRC Module */
  /*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�
  ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 
	/* LED �˿ڳ�ʼ�� */ 
  //LED_GPIO_Config();
	LED_Init();// �����и�Bug������ʼ��LED ��Ļ������Ӧ����IO�����ƶ���LCD��ʼ�����
	/*SramInit*/
	//FSMC_SRAM_Init();
	
	/* ��ʼ������ */
	//Touch_Init();
	
	/*����ʼ��*/
	//bsp_InitLCD();
	TFTLCD_Init();
	//LCD_Init();		//LCD��ʾ��ʼ��	
	//LCD_Reginit();
	
	//������ʼ��
	 KEY_Init();
	 //IO��ʼ��

	//���ڳ�ʼ��
	USART1_Init(9600);
	USART2_Init();

	/* ��ʼ��gui */
/*
  UG_Init(&gui,void(*)(UG_S16,UG_S16,UG_COLOR))_HW_DrawPoint,240,320);
	UG_DriverRegister(DRIVER_DRAW_LINE,(void*)_HW_DrawLine);
	UG_DriverRegister(DRIVER_FILE_FRAME,(void*)_HW_FillLine);
	UG_DriverEnable(DRIVER_DRAW_LINE);
	UG_DricerEnable(DRIVER_FILL_FRAME);
*/

	//spi��ʼ��
	SPI_Flash_Init();

}

/*
 * ��������SysTick_init
 * ����  ������SysTick��ʱ��
 * ����  ����
 * ���  ����
 */
void SysTick_init(void)
{
  /* ��ʼ����ʹ��SysTick��ʱ�� */
  SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ);
  
  /*  ����1ms �ж�һ�Σ���os��Ƶ��Ϊ1000hz */
	if (SysTick_Config(SystemCoreClock/OS_CFG_TICK_RATE_HZ))	
	{ 
		/* Capture error */ 
		while (1);
	}
}


/* --------------------------------------end of file--------------------------------------- */
