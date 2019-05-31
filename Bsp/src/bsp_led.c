/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;	    		 //LED1-->PE.5 ????, ????
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //???? ,IO????50MHz
	 //GPIO_SetBits(GPIOE,GPIO_Pin_5); 			 
	 //GPIO_SetBits(GPIOE,GPIO_Pin_1); 			
	 //GPIO_SetBits(GPIOE,GPIO_Pin_6); 	
	
	
	
	
}
 


/**
* @brief  LED���ڷ�ת����systick���ڵ���
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
