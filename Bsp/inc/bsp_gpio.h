#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H


typedef enum e_InOutID{
    //Output
	OUTPUT_START						=1,
	OUTPUT_BackLight,
	OUTPUT_Beeper,
	OUTPUT_END,
    INPUT_START						=100,
    INPUT_END,						
}eInOut;

void bsp_InOutInit(void);
u8 bsp_GetInputState(u8 InOutID, eOnOff* pOnOff);
u8 bsp_SetOutputState(u8 InOutID, eOnOff bOn);

#endif //_BSP_GPIO_H

