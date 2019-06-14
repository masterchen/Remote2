#ifndef	_APP_H_
#define	_APP_H_

#include "bsp.h" 
#include "os.h"  		
#include "os_type.h"
#include "ugui.h"
#include "malloc.h"
typedef enum e_KeyType
{  
	Key_SpeedAdj = 0,//调速
	Key_LegCntSwitch,//切换俺就
	Key_BackUp,//背部上折
	Key_BackDown,//背部下折
	Key_WaistUp,//腰部上折
	Key_WaistDown,//腰部下折
	Key_LegUp,//腿板上升
	Key_LegDown,
	Key_Trendelenburg,//头低脚高 前倾
	Key_AntiTrendelenburg,//后倾
	key_Up,//升高
	Key_Down,//降低
	Key_LeftLean,//左倾
	Key_RightLean,//右倾
	Key_HeadForward,//头部平移
	Key_FootForward,//脚部平移
	Key_ReverseFunc,//正反向按钮
	Key_UnLock,//解锁
	Key_Lock,//锁定
	Key_Start,//启动
	Key_Stop,//停止
	Key_Reset,//零位
	Key_Memo,//记忆按钮
	Key_PageUp,
	Key_PageDown,
	Key_Func1,
	Key_Func2,
	key_ReadData,
	Key_Max
}eKeyType;

typedef struct s_KeyMap{
	eKeyType KeyType;/*按键类型， 这里把命令也当作按键*/
	unsigned  char KeyCode;/*按键代码*/
	u8 CmdCode;/* 对应的CmdCode */
}sKeyMap;

typedef struct s_KeyMap2{
	eKeyType KeyType;/*按键类型， 这里把命令也当作按键*/
	unsigned  char KeyCode;/*按键代码*/
}sKeyMap2;




/*******************设置任务优先级*******************/
#define STARTUP_TASK_PRIO       3
#define	TASK_LED1_PRIO			    4
#define	TASK_KEY_PRIO			    5

#define	TASK_GUI_PRIO				    7

/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE   80 
#define	TASK_LED1_STK_SIZE		  80  
#define	TASK_GUI_STK_SIZE       500
#define TASK_KEY_STK_SIZE 80

/**************** 用户任务声明 *******************/
void Task_Start(void *p_arg);
void Task_LED1(void *p_arg);
void Task_LED2(void *p_arg);
void Task_LED3(void *p_arg);
void Task_Key(void *p_arg);
void Task_GUI(void *p_arg);
void Task_GUI_Touch(void *p_arg);

/**************** 界面控制函数定义******************/

void UGUI_WindowInit(void);
void UGUI_ShowSpash(void);

void UGUI_ShowMainWindow(void);

void UGUI_ShowSubWindow(void);


void UGUI_WindowStateHandler(eKeyType event);

u8 appCOM_SendCmdCode(u8 _CmdCode);



#endif	//_APP_H_
