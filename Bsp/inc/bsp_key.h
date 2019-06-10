#ifndef __KEY_H
#define __KEY_H	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�������� ��������		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/11/28 
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.1�޸�˵��
//�޸İ���ɨ�躯����ʹ�����������֧��SWD���ء�
//////////////////////////////////////////////////////////////////////////////////	 

typedef union u_KeyCode{
	unsigned  short KeyValue; 
	struct{
		unsigned char KeyRow;
		unsigned char KeyCol:3;
		unsigned char State:2;
	}Value;
}uKeyCode;

typedef enum e_KeyState{
	KeyState_Invalid,
	KeyState_Pressed,
	KeyState_Repeat,
	KeyState_Release,
}eKeyState;


typedef enum 
{  
	Button_KEY0 = 0,
	Button_KEY1 = 1,
	Button_WAKEUP = 2
} Button_TypeDef;





typedef struct  
{   GPIO_TypeDef*       BUTTON_PORT ;
    uint16_t            BUTTON_PIN ;
    GPIOMode_TypeDef    BUTTON_MODE;
}Button_PinModeDef;	

	 
void KEY_Init(void);//IO��ʼ��
u8 Key_Scan(void);

#endif

