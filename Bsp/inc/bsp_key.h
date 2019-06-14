#ifndef __KEY_H
#define __KEY_H	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/11/28 
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.1修改说明
//修改按键扫描函数，使整个代码可以支持SWD下载。
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


	 
void KEY_Init(void);//IO初始化
u8 Key_Scan(void);

#endif

