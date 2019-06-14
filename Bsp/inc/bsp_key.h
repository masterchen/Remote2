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


	 
void KEY_Init(void);//IO��ʼ��
u8 Key_Scan(void);

#endif

