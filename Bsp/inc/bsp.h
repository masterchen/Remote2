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

void SysTick_init(void);
extern void BSP_Init(void);
extern u8 KEY_Scan(void); //°´¼üÉ¨Ãèº¯Êý			

#endif /* __INCLUDES_H__ */



