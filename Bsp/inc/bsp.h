#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

#include "stm32f10x.h"
#include "os_cfg_app.h"
#include "bsp_led.h"
#include "bsp_touch.h"
#include "bsp_usart.h"
#include "bsp_ili93xx.h"
#include "bsp_sram.h"
void SysTick_init(void);
extern void BSP_Init(void);

#endif /* __INCLUDES_H__ */



