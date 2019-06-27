#include "stm32f10x.h"
#include "bsp_key.h"
#include "sys.h" 
#include "delay.h"	  
#include "bsp_usart.h"	  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�������� ��������		   
//������̳:www.openedv.com
//�޸�����:2011/11/28 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ALIENTEK  2009-2019
//All rights reserved
//********************************************************************************
/*
#define SUB_KEY_MASK			(0x03e0)
#define SUB_INVALID_KEY		(SUB_KEY_MASK>>8)
#define KEY_SENSITIVITY			(5)
#define KEY_REPEAT_CNT			(25)
*/
GPIO_InitTypeDef GPIO_InitStructure;


typedef struct{
	unsigned char KeyStateIndex; ����// ��ǰ״̬������
	unsigned char KeyEnterState; ����//���¡��س�����ʱת���״̬������
	unsigned char KeyBackState; ����// ���¡��˻ء���ʱת���״̬������
	unsigned char KeyLeftState; ����//���¡����¡���ʱת���״̬������
	unsigned char KeyRightState; ����// ���¡����ϡ���ʱת���״̬������
	unsigned char KeySetState;     //�������ü�ת���״̬������
	void (*CurrentOperate)(); ����//��ǰ״̬Ӧ��ִ�еĹ��ܲ���
}KbdTabStruct;

#define SIZE_OF_KEYBD_MENU     55 ����/ / �˵��ܳ��� 

void (*KeyFuncPtr)() ;

KbdTabStruct const MainKeyTab[SIZE_OF_KEYBD_MENU]={

//index,leftsur,left,right,rightsur,mix
{0,1,2,3,4,5,(*fastmem_select_mem_window)},
{1,2,3,4,5,0,(*fastmem_select_exe_window)},
{2,3,4,5,0,1,(*UGUI_ShowSubWindow)},
{3,4,5,0,1,2,(*UGUI_ShowMainWindow)},
{4,5,0,1,2,3,(*UGUI_ShowSpasha)}��
{5,0,1,2,3,4,(*UGUI_ShowSpash)},
};

void GetKeylnput (int key)
{

  static int Key_Fun = 0;

��switch(key){
	
�� case Key_Func1: 	   //�س���,�ҳ��µĲ˵�״̬���
		Key_Fun = MainKeyTab[Key_Fun].KeyEnterState ;
������	  break;
�� case Key_Func2: 		//���¼�,�ҳ��µĲ˵�״̬���
		Key_Fun = MainKeyTab[Key_Fun].KeyBackState ;
����		break;
   case Key_PageUp: 	  //���ϼ�,�ҳ��µĲ˵�״̬���
		Key_Fun = MainKeyTab[Key_Fun].KeyLeftState ;
������	  break;
�� case Key_PageDown:		  //���˼�,�ҳ��µĲ˵�״̬���
		Key_Fun = MainKeyTab[Key_Fun].KeyRightState ;
������	  break;
�� case 0;
������ return; ����// ����Ĵ���
������ break;
������}

�� KeyFuncPtr = MainKeyTab[Key_Fun].CurrentOperate ;

��(*KeyFuncPtr)() ; ����// ִ�е�ǰ�����Ĳ���
}













/*
const sKeyMap _sKeyMap[]={
	{cmdkey_multifunction1,   0x33 }       //k24
	{cmdkey_menu_one,                     //k25
	{cmdkey_menu2,                    //k26
	{cmdkey_multifunction2,           //k27
	{cmdkey_speed_regulation          //k22
	{cmdkey_memory,               0x22}       //k23	
	{cmdkey_lean_backward,            //k19
	{cmdkey_lean_forward,             //k20
	{cmdkey_lean_left,                //k16
	{cmdkey_lean_right,               //k18
	{cmdkey_rise,                     //k21
	{cmdkey_drop,                     //k17
	{cmdkey_back_rise,                //k13
	{cmdkey_back_drop,                //k15
	{cmdkey_waist_rise,               //k10
	{cmdkey_waist_drop,               //k12
	{cmdkey_leg_rise,                 //k7
	{cmdkey_leg_drop,                  //k9
	{cmdkey_lock,                      //k6
	{cmdkey_unlock,                    //k4
	{cmdkey_start,                     //k1
	{cmdkey_stop,                      //k2
	{cmdkey_forward_reverse,           //k5
	{cmdkey_zero,                      //k3
};
*/
void KEY_Init(void) //IO��ʼ��
{
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
		 
}

u8 Key_Scan(void)
{ 
	 u8 keyValue=0;
		//u8 keyValue[5]={0x00,0x00,0x00,0x00,0x00};
		//int x1,x2,x3,x4,x5;
	//GPIO_Write(GPIOC,(GPIOC->ODR & 0x0000));//让PC5-9输出高电�?
	
	GPIO_Write(GPIOC,((GPIOC->ODR & 0xfc1f )| 0x03e0));//让PC5-9输出高电�?
  if((GPIOC->IDR & 0x001f)==0x0000)//若PC0-4全为0，则没有按键按下
	 {
		delay_ms(2);
	  //return -1;
	 }
  else
	{
		delay_ms(3);
		if((GPIOC->IDR & 0x001f)==0x0000)//若PC0-4全为0，则刚KeyRow刚是抖动产生
		return 0;	
	}


	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0020);//?PC5-9??0001,PC5?,??PC0-4	
	  switch(GPIOC->IDR&0x001f)
		{

			case 0x0001 : 
				 keyValue=0x03;
	         		
				 //keyValue[1]=0x0021;			      
				 break;
			case 0x0002 : 
				 keyValue=0x04;
	         		
				 //keyValue[1]=0x0022;			      
				 break;
			case 0x0004 : 
				 keyValue=0x05;
	         		
				 //keyValue[1]=0x0024;			      
				 break;
			case 0x0008 : 
				 keyValue=0x06;
	         		
				 //keyValue[1]=0x0028;			      
				 break;
			case 0x0010 : 
				keyValue=0x07;
	         		
				//keyValue[1]=0x0030;
				break;	
 		
		}
		
		
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0040);//?PC5-9??0001,PC5?,??PC0-4	
	  switch(GPIOC->IDR&0x001f)
		{

			case 0x0001 : 
				keyValue=0x08;
	         		
				 //keyValue[2]=0x0041;			      
				break;
			case 0x0002 : 
				keyValue=0x09;
	         		
				 //keyValue[2]=0x0042;			      
				break;
			case 0x0004 : 
				keyValue=0x0a;
	         		
				 //keyValue[2]=0x0044;			      
				break;	
			case 0x0008 : 
				keyValue=0x0b;
	         		
				 //keyValue[2]=0x0048;			      
				break;	
			case 0x0010 : 
				 keyValue=0x0c;
	         		
				 //keyValue[2]=0x0050;			      
				 break;			
		}
		
	  GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0080);//?PC5-9??0001,PC5?,??PC0-4	
	  switch(GPIOC->IDR&0x001f )
		{

			case 0x0001 : 
				 keyValue=0x0d;
	         		
				 //keyValue[3]=0x0081;			      
				 break;
			case 0x0002 : 
				 keyValue=0x0e;
	         		
				 //keyValue[3]=0x0082;			      			      
				 break;
			case 0x0004 : 
				 keyValue=0x0f;
	         		
				 //keyValue[3]=0x0084;			      
				 break; 
			case 0x0008 : 
				 keyValue=0x10;
	         		
				 //keyValue[3]=0x0088;			      
				 break;	
			case 0x0010 : 
				 keyValue=0x11;
	         		
				 //keyValue[3]=0x0090;			      
				 break;		
		}
		
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0100);//?PC5-9??0001,PC5?,??PC0-4	
	  switch(GPIOC->IDR&0x001f )
		{

			case 0x0001 : 
				 keyValue=0x12;
				 //keyValue[3]=0x0090;			      
				 break;
			case 0x0002 : 
				 keyValue=0x13;
				 break;
			case 0x0004 : 
				 keyValue=0x14;
				 break; 
			case 0x0008 : 
				 keyValue=0x15;
	         		
				 break;
			case 0x0010 : 
				 keyValue=0x16;
	         		
				 break;
		}
	
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| 0x0200);//?PC5-9??0001,PC5?,??PC0-4	
	  switch(GPIOC->IDR&0x001f)
		{
			case 0x0001 : 
				 keyValue=0x17;
	         		
				 break;
			case 0x0002 : 
				 keyValue=0x18;
	         		
				 break;
			case 0x0004 : 
				 keyValue=0x19;
	         		
				 break;
			case 0x0008 : 
				 keyValue=0x1a;
	         		
				 break; 
		  case 0x0010 : 
				 keyValue=0x1b;
	         		
				 break;
		}	

	return keyValue;

}




/*
typedef struct{
	unsigned char CurkeyIndex;
	unsigned char LeftsurState;
	unsigned char LeftState;	
	unsigned char RightState;	
	unsigned char RightsurState;
	void (*CurOperate)();
}MainKeyTable;
*/





/*
KbdTabStruct const KeyTab[SIZE_OF_KEYBD_MENU] =
{

//index,enter,esc,left,right

��{0 ,0 ,0 ,1 ,0 , MainJob1 } ,
��{1 ,7 ,2 ,8 ,0 , Dsp Point} , ����/ / ��һ��
��{2 ,1 ,3 ,8 ,0 , DspCurve}, ����/ / ��һ��
��{3 ,2 ,4 ,36 ,0 ,Dsp Kout} , ����/ / ��һ��
��{4 ,3 ,5 ,50 ,0 , DisCloseDown } , ����/ / ��һ��


��{5 ,4 ,6 ,8 ,0 ,  ModifyPoint} , ����/ / ��һ��
��{6 ,5 ,7 ,52 ,0 , SetCloseDown } , ����/ / ��һ��
��{7 ,6 ,1 ,0 ,0 , Cancel} , ����/ / ��һ��

��{52 ,53 ,53 ,0 ,1 ,  OkSetCloseDown1} ,
��{53 ,52 ,52 ,0 ,1 , OkSetCloseDown2} ,
��{54 ,0 ,0 ,0 ,0 , Disable} ,

} ;
��void Get Keylnput (void)
{
��switch ( status &0xf0)
����{
����case 0xe0 :/ / �س���,�ҳ��µĲ˵�״̬���
��������
        KeyFuncIndex = KeyTab[ KeyFuncIndex ] . KeyCrState ;
��������
������break ;
����case 0xb0 :/ / ���¼�,�ҳ��µĲ˵�״̬���
��������
��������KeyFuncIndex = KeyTab [ KeyFuncIndex ] . KeyDn2State ;
��������
����break ;
   case 0xd0 :/ / ���ϼ�,�ҳ��µĲ˵�״̬���


��������
��������KeyFuncIndex = KeyTab [ KeyFuncIndex ] . KeyUp2State ;
��������
������break ;
����case 0x70 :/ / ���˼�,�ҳ��µĲ˵�״̬���
��������
��������KeyFuncIndex = KeyTab [ KeyFuncIndex ] . KeyBack2State ;
��������
������break ;
����case 0 ;
��������return ; ����/ / ����Ĵ���
��������break ;
������}
����KeyFuncPt r = KeyTab[ KeyFuncIndex ] . CurrentOperate ;
����( * KeyFuncPt r) () ; ����/ / ִ�е�ǰ�����Ĳ���
}

*/
















/*	
int Key_Scan(void)
{
	static u8 key_up = 1;
	key_up=1;
	static u8 oldKeyCode = 0xff;
	static u8 LastKeyCode = 0;
	static u16 ScanCnt = 0;
	static u8 KeyCol = 0x0020;
	u16 realKeyCode = 0;
	static uKeyCode LastKeyCodeUnion;
	uKeyCode KeyCodeUnion;

	//  
	GPIO_Write(GPIOC,((GPIOA->ODR & 0x00ff )| 0x03C0));//让PC5-9输出高电�?
		if((GPIOC->IDR & 0x001f)==0x0000)//若PC0-4全为0，则没有按键按下
		return -1;
	else
	{
		delay_ms(5);//延时消抖
		if((GPIOC->IDR & 0x001f)==0x0000)//若PC0-4全为0，则刚KeyRow刚是抖动产生
		return -1;	
	}
	GPIO_Write(GPIOC,(GPIOC->ODR & 0xfc1f )| KeyCol);//让PC5-9输出0001，PC5高，检测PC0-4
	KeyCodeUnion.Value.KeyCol = KeyCol;
	KeyCodeUnion.Value.KeyRow = GPIO_ReadInputData(GPIOC);
	if(KeyCodeUnion.Value.KeyRow == LastKeyCode){
		ScanCnt++;
	}
	else{
		ScanCnt = 0;
	}
		
	if(ScanCnt>KEY_SENSITIVITY){
		GPIO_Write(GPIOC,0x0000);

	if(KeyCodeUnion.Value.KeyRow == SUB_INVALID_KEY){
		GPIO_Write(GPIOC,0x0000);
		KeyCol = KeyCol + 0x0020;
		if(KeyCol > 0x0200)KeyCol=0x0020;
	}

	LastKeyCode = KeyCodeUnion.Value.KeyRow;
	if(oldKeyCode != KeyCodeUnion.Value.KeyRow 
	&& ScanCnt>KEY_SENSITIVITY
	&& oldKeyCode == (u8)SUB_INVALID_KEY
	&& KeyCodeUnion.Value.KeyRow != (u8)SUB_INVALID_KEY){
	
		KeyCodeUnion.Value.State = KeyState_Pressed;
		realKeyCode = KeyCodeUnion.KeyValue;
		oldKeyCode = KeyCodeUnion.Value.KeyRow;
		LastKeyCodeUnion = KeyCodeUnion;
		ScanCnt = 0;
		appCOM_SendKeyCode(realKeyCode);
	}
	else if(oldKeyCode != KeyCodeUnion.Value.KeyRow 
		&& ScanCnt>KEY_REPEAT_CNT){
		
		LastKeyCodeUnion.Value.State = KeyState_Release;
		realKeyCode = LastKeyCodeUnion.KeyValue;
		oldKeyCode = KeyCodeUnion.Value.KeyRow;
		LastKeyCodeUnion = KeyCodeUnion;
		ScanCnt = 0;
		appCOM_SendKeyCode(realKeyCode);
	}
	else if(KeyCodeUnion.Value.KeyRow != (u8)SUB_INVALID_KEY
		 && KeyCodeUnion.Value.KeyRow == oldKeyCode
		 && ScanCnt>KEY_REPEAT_CNT){
		 ScanCnt = 0;
		LastKeyCodeUnion.Value.State = KeyState_Repeat;
		appCOM_SendKeyCode(LastKeyCodeUnion.KeyValue);
	}
}

*/










