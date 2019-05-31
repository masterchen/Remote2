/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   led应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;	    		 //LED1-->PE.5 ????, ????
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //???? ,IO????50MHz
	 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 			 
	 //GPIO_SetBits(GPIOE,GPIO_Pin_1); 			
	 //GPIO_SetBits(GPIOE,GPIO_Pin_6); 	
	
	
	
	
}
 


/**
* @brief  LED周期反转，给systick周期调用
* @param  None
* @retval None
*/ 
void LED_Toggle(void)
{
  static uint32_t LED_ticks = 0;
    
  if ( LED_ticks++ > 100 )
  {
    LED_ticks = 0;
    
    /* toggle LED1..4 each 100ms */
    LED1_TOGGLE;
    LED2_TOGGLE;
    LED3_TOGGLE;
	}
}
/*********************************************END OF FILE**********************/
