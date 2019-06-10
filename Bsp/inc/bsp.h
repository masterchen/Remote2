#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__



#include "stm32f10x.h"
#include "os_cfg_app.h"
#include "bsp_led.h"
#include "bsp_touch.h"
#include "bsp_usart.h"
#include "bsp_ili93xx.h"
#include "bsp_sram.h"
#include "bsp_key.h"
#include "bsp_spi.h"
#include "bsp_flash.h"
#include "os.h"

typedef  void	*(*BSPFuncHook)(void *Arg);



#define BSP_CreateMutex(p_mutex, name, p_err)   OSMutexCreate (p_mutex, name, p_err)
#define BSP_Lock(p_mutex)\
{\
    OS_ERR _err;\
    OSMutexPend (p_mutex, 0, OS_OPT_PEND_BLOCKING, (CPU_TS *)0, &_err);\
}\

#define BSP_UnLock(p_mutex)\
{\
    OS_ERR _err;\
    OSMutexPost (p_mutex, OS_OPT_PEND_BLOCKING, &_err);\
}\



void SysTick_init(void);
extern void BSP_Init(void);
extern u8 KEY_Scan(void); //°´¼üÉ¨Ãèº¯Êý			





#define MCULOG_LENGTH_MAX       50    
     

#endif /* __INCLUDES_H__ */



