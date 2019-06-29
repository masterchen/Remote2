/* Includes 
-----------------------------------------------------------------------------*/
#include "app.h"


/* Definitions 
-----------------------------------------------------------------------------*/

#define GPIO_DETETION_EN     DEF_ENABLED//DEF_DISABLED//DEF_ENABLED

/* Private variables 
-----------------------------------------------------------------------------*/
static OS_TMR  Ten_ms_tmr;       //��ʱ��1

struct tagTwinkleBeeper
{
    u8 bTwinkle:1;
    u8 t_ucTimes:7;
    u8 t_ucInterval;
    u8 s_ucInterval;
}t_BeeperParm;

struct tagBacklightPWM
{
    u8 bTwinkle:1;
    u8 t_ucTimes:7;
    u8 t_ucInterval;
    u8 s_ucInterval;
}t_BacklightPWM;


/* Public variables 
-----------------------------------------------------------------------------*/


/* Private function prototypes 
-----------------------------------------------------------------------------*/
/*
*******************************************************************************
*   �� �� ��: 
*   ����˵��: 
*   ��    ��:  ��
*   �� �� ֵ: ��
*******************************************************************************
*/
void appMon_CheckBeeper(void)
{
    if(t_BeeperParm.bTwinkle)
    { 
        if( t_BeeperParm.t_ucInterval == t_BeeperParm.s_ucInterval )
        {
            //ControllerBeeper((BitAction)FALSE);
            bsp_SetOutputState(OUTPUT_Beeper,       _OFF_);
            if(t_BeeperParm.t_ucTimes!=0)
            {
                if(--t_BeeperParm.t_ucTimes == 0)     
                    t_BeeperParm.bTwinkle = FALSE;
            }
        }
        else if( (t_BeeperParm.t_ucInterval>>1) == t_BeeperParm.s_ucInterval)
        {
            t_BeeperParm.t_ucInterval = 0;
            //ControllerBeeper((BitAction)TRUE);
            bsp_SetOutputState(OUTPUT_Beeper,       _ON_);
        }
        t_BeeperParm.t_ucInterval++;
    }
}


void appMon_CheckBackLightPwm(void)
{
    if(t_BacklightPWM.bTwinkle)
    { 
        if( t_BacklightPWM.t_ucInterval == t_BacklightPWM.s_ucInterval )
        {
            //ControllerBeeper((BitAction)FALSE);
            bsp_SetOutputState(OUTPUT_BackLight,       _OFF_);
            if(t_BacklightPWM.t_ucTimes!=0)
            {
                if(--t_BacklightPWM.t_ucTimes == 0)     
                    t_BacklightPWM.bTwinkle = FALSE;
            }
        }
        else if( (t_BacklightPWM.t_ucInterval>>1) == t_BacklightPWM.s_ucInterval)
        {
            t_BacklightPWM.t_ucInterval = 0;
            //ControllerBeeper((BitAction)TRUE);
            bsp_SetOutputState(OUTPUT_BackLight,       _ON_);
        }
        t_BacklightPWM.t_ucInterval++;
    }
}



/*
*******************************************************************************
*   �� �� ��: 
*   ����˵��: 
*   ��    ��:  ��
*   �� �� ֵ: ��
*******************************************************************************
*/
u32 appMon_SetBeeper(u8 ucTimes, u8 ucInterval)
{
    u32 ErrCode = OS_ERR_NONE;
    t_BeeperParm.s_ucInterval = ucInterval;
    t_BeeperParm.t_ucInterval = (ucInterval<<1);
    t_BeeperParm.t_ucTimes = ucTimes;    
    t_BeeperParm.bTwinkle = ucInterval!=0;
    if(!ucInterval)bsp_SetOutputState(OUTPUT_Beeper,       _OFF_);
    return ErrCode;
}


u32 appMon_SetBackLightPwm(u8 ucTimes, u8 ucInterval)
{
    u32 ErrCode = OS_ERR_NONE;
    t_BacklightPWM.s_ucInterval = ucInterval;
    t_BacklightPWM.t_ucInterval = (ucInterval<<1);
    t_BacklightPWM.t_ucTimes = ucTimes;    
    t_BacklightPWM.bTwinkle = ucInterval!=0;
    if(!ucInterval)bsp_SetOutputState(OUTPUT_BackLight,       _OFF_);
    return ErrCode;
}


/*
*******************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��	  ��:  ��
*	�� �� ֵ: ��
*******************************************************************************
*/
void appMon_handler(void)
{
	//appMon_CheckBeeper();
	//appMon_CheckBackLightPwm();

	///TODO::   can be used as watchdog for  backlight control
}


/*
*******************************************************************************
*	�� �� ��: 
*	����˵��: 
*	��	  ��:  ��
*	�� �� ֵ: ��
*******************************************************************************
*/
u8 appMon_Init(void)
{
    OS_ERR  err;
    u8 bRet = TRUE;
    
    OSTmrCreate((OS_TMR     *)&Ten_ms_tmr,        //��ʱ��1
                (CPU_CHAR   *)"10ms timer",       //��ʱ������
                (OS_TICK     )(100/OSTmrUpdateCnt),           //1ms
                (OS_TICK     )(100/OSTmrUpdateCnt),          //1ms
                (OS_OPT      )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)appMon_handler,//��ʱ��1�ص�����
                (void       *)0,            //����Ϊ0
                (OS_ERR     *)&err);        //���صĴ�����  
                
        if(err != OS_ERR_NONE){
            printf("Failed to OSTmrCreate");
        }
        OSTmrStart(&Ten_ms_tmr,&err);
        if(err != OS_ERR_NONE){
            printf("Failed to OSTmrStart, err=%d", err);
        }
    return bRet;
}



