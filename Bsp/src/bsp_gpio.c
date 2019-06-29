#include "bsp.h"

typedef struct s_InOutDriver{
	GPIO_TypeDef * 	Port;
	u16				Pin;
	GPIOMode_TypeDef InOutMode;
    GPIOPuPd_TypeDef PdPu;
	BitAction bValid;
	u8 InOutID;
}InOutDriver;



static const InOutDriver s_InOutDriverTab[] = 
{
  
    {GPIOE,  GPIO_Pin_5,     GPIO_Mode_Out_PP,   GPIO_PuPd_UP,       Bit_RESET,    OUTPUT_BackLight},//
    {GPIOE,  GPIO_Pin_4,  	GPIO_Mode_Out_PP,   GPIO_PuPd_UP,       Bit_SET,    OUTPUT_Beeper},//
};

#define	INOUT_NUM	(sizeof(s_InOutDriverTab)/sizeof(InOutDriver))

static OS_MUTEX sInOutMutex;
static u8 sFunctionID = 0;



void bsp_InOutInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 i;
    OS_ERR  err = OS_ERR_NONE;



    BSP_CreateMutex(&sInOutMutex, "GPIOInOutMutex", &err);
    
    if(err != OS_ERR_NONE)
    {
        if(err != OS_ERR_NONE)
        return;
    }
    
	for(i=0; i<INOUT_NUM; i++)
	{
	    if(s_InOutDriverTab[i].Port == NULL) continue;
        
		RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(s_InOutDriverTab[i].Port), ENABLE);	/* 打开GPIO时钟 */

		GPIO_InitStructure.GPIO_Mode = s_InOutDriverTab[i].InOutMode;		/* 设为输出口 */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

		GPIO_InitStructure.GPIO_Pin = s_InOutDriverTab[i].Pin;
		GPIO_Init(s_InOutDriverTab[i].Port, &GPIO_InitStructure);
		if(s_InOutDriverTab[i].bValid)
			GPIO_ResetBits(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin);
		else
            GPIO_SetBits(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin);
	}
 
}


u8 bsp_GetInputState(u8 InOutID, eOnOff* pOnOff)
{
    BitAction bState, bLastState;
    u8 i, trycnt;

    BSP_Lock(&sInOutMutex);

	for(i=0; i <INOUT_NUM; i++)
	{
		if(s_InOutDriverTab[i].InOutID == InOutID)
			break;
	}
	
	if(i>=INOUT_NUM)
	{
        BSP_UnLock(&sInOutMutex);
		return FALSE;
	}
    #if 1
    
    //bState = (BitAction)GPIO_ReadInputDataBit(s_InOutDriverTab[i].port, s_InOutDriverTab[i].pin);
    bLastState = (BitAction)GPIO_ReadInputDataBit(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin);
    trycnt=1;
    do
    {
        bState = (BitAction)GPIO_ReadInputDataBit(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin);
        trycnt--;
        if(bLastState != bState)
        {
            trycnt=1;
            bLastState = bState;
        }
    }while(trycnt);

    if(s_InOutDriverTab[i].bValid)
        *pOnOff = (eOnOff)bState;
    else
        *pOnOff = (eOnOff)!bState;
    #else
    #endif
    
    BSP_UnLock(&sInOutMutex);
	return TRUE;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetOutputState
*	功能说明: 
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 bsp_SetOutputState(u8 InOutID, eOnOff bOn)
{
	u8 i;
//    BitAction bState = Bit_RESET;

    BSP_Lock(&sInOutMutex);

	for(i=0; i <INOUT_NUM; i++)
	{
		if(s_InOutDriverTab[i].InOutID == InOutID)
			break;
	}

    bOn = (eOnOff)(bOn & 0x01);
	if(i>=INOUT_NUM)
	{
        BSP_UnLock(&sInOutMutex);
		return FALSE;
	}
    //PrintInfo("InOutID=%d, bState=%d", InOutID, bState);
	if(bOn == _ON_)
		GPIO_WriteBit(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin, s_InOutDriverTab[i].bValid);
	else
		GPIO_WriteBit(s_InOutDriverTab[i].Port, s_InOutDriverTab[i].Pin, (BitAction)!s_InOutDriverTab[i].bValid);

    BSP_UnLock(&sInOutMutex);
    return TRUE;
}


