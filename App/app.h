#ifndef	_APP_H_
#define	_APP_H_

#include "bsp.h" 
#include "os.h"  		
#include "os_type.h"
#include "ugui.h"
#include "malloc.h"
typedef enum e_KeyType
{  
	Key_SpeedAdj = 0,//����
	Key_LegCntSwitch,//�л�����
	Key_BackUp,//��������
	Key_BackDown,//��������
	Key_WaistUp,//��������
	Key_WaistDown,//��������
	Key_LegUp,//�Ȱ�����
	Key_LegDown,
	Key_Trendelenburg,//ͷ�ͽŸ� ǰ��
	Key_AntiTrendelenburg,//����
	key_Up,//����
	Key_Down,//����
	Key_LeftLean,//����
	Key_RightLean,//����
	Key_HeadForward,//ͷ��ƽ��
	Key_FootForward,//�Ų�ƽ��
	Key_ReverseFunc,//������ť
	Key_UnLock,//����
	Key_Lock,//����
	Key_Start,//����
	Key_Stop,//ֹͣ
	Key_Reset,//��λ
	Key_Memo,//���䰴ť
	Key_PageUp,
	Key_PageDown,
	Key_Func1,
	Key_Func2,
	key_ReadData,
	Key_Max
}eKeyType;

typedef struct s_KeyMap{
	eKeyType KeyType;/*�������ͣ� ���������Ҳ��������*/
	unsigned  char KeyCode;/*��������*/
	u8 CmdCode;/* ��Ӧ��CmdCode */
}sKeyMap;

typedef struct s_KeyMap2{
	eKeyType KeyType;/*�������ͣ� ���������Ҳ��������*/
	unsigned  char KeyCode;/*��������*/
}sKeyMap2;




/*******************�����������ȼ�*******************/
#define STARTUP_TASK_PRIO       3
#define	TASK_LED1_PRIO			    4
#define	TASK_KEY_PRIO			    5

#define	TASK_GUI_PRIO				    7

/************����ջ��С����λΪ OS_STK ��************/
#define STARTUP_TASK_STK_SIZE   80 
#define	TASK_LED1_STK_SIZE		  80  
#define	TASK_GUI_STK_SIZE       500
#define TASK_KEY_STK_SIZE 80

/**************** �û��������� *******************/
void Task_Start(void *p_arg);
void Task_LED1(void *p_arg);
void Task_LED2(void *p_arg);
void Task_LED3(void *p_arg);
void Task_Key(void *p_arg);
void Task_GUI(void *p_arg);
void Task_GUI_Touch(void *p_arg);

/**************** ������ƺ�������******************/

void UGUI_WindowInit(void);
void UGUI_ShowSpash(void);

void UGUI_ShowMainWindow(void);

void UGUI_ShowSubWindow(void);


void UGUI_WindowStateHandler(eKeyType event);

u8 appCOM_SendCmdCode(u8 _CmdCode);



#endif	//_APP_H_
