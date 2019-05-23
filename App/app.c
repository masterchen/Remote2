#include "app.h"

#include "bsp_touch.h"

void Task_GUI(void *p_arg);
extern void TOUCH_Scan(void);

extern	OS_TCB	StartUp_TCB;					

static  OS_TCB	LED1_TCB;		   				
static	CPU_STK	LED1_Stk[TASK_LED1_STK_SIZE];	   

static  OS_TCB	GUI_TCB;		   					
static	CPU_STK	GUI_Stk[TASK_GUI_STK_SIZE];

/* 
 * ��������Task_Start
 * ����  : ��������
 *			   ���ȼ�Ϊ3��
 *		     ����LED1��LED2��LED3������
 * ����  ����
 * ���  : ��
 */
void Task_Start(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;	

	//��������LED1	
	OSTaskCreate((OS_TCB     *)&LED1_TCB,					            // ������ƿ�ָ��          
               (CPU_CHAR   *)"LED1",						            // ��������
               (OS_TASK_PTR )Task_LED1,					            // �������ָ��
               (void       *)0,							                // ���ݸ�����Ĳ���parg
               (OS_PRIO     )TASK_LED1_PRIO,				        // �������ȼ�
               (CPU_STK    *)&LED1_Stk[0],				          // �����ջ����ַ
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE/10,		      // ��ջʣ�ྯ����
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE,			      // ��ջ��С
               (OS_MSG_QTY  )0,							                // �ɽ��յ������Ϣ������
               (OS_TICK     )0,							                // ʱ��Ƭ��תʱ��
               (void       *)0,							                // ������ƿ���չ��Ϣ
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	        // ����ѡ��
               (OS_ERR     *)&err);						              // ����ֵ
							 
		OSTaskCreate((OS_TCB     *)&GUI_TCB,					  
               (CPU_CHAR   *)"GUI",						
               (OS_TASK_PTR )Task_GUI,				
               (void       *)0,							
               (OS_PRIO     )TASK_GUI_PRIO,				
               (CPU_STK    *)&GUI_Stk[0],				
               (CPU_STK_SIZE)TASK_GUI_STK_SIZE/10,		
               (CPU_STK_SIZE)TASK_GUI_STK_SIZE,			
               (OS_MSG_QTY  )0,						
               (OS_TICK     )0,							
               (void       *)0,							
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	 
               (OS_ERR     *)&err);	
               
  //����ɾ���Լ�	
	OSTaskDel(&StartUp_TCB,&err);							 
}


/* 
 * ��������Task_LED1
 * ����  : LED����1��
 *		     ���ȼ�Ϊ4��
 *		     ��100ms�ļ���������ر�LED1
 * ����  ����
 * ���  : ��
 */
void Task_LED1(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;                	

  while (1)
  {
		//GUI_TOUCH_Exec();										//10msһ��GUI���ô�����
		//TOUCH_Scan();											//10msһ��GUI���ô�����
	{ 
		LCD_ShowString(60,130,200,16,16,"Show me1 !");
		LCD_Fill( 10, 100, 10, 130 , RED);

		OSTimeDly(100,OS_OPT_TIME_DLY,&err);
		//LCD_ShowString(60,130,200,16,16,"Show me2 !");	
	}	
		printf("Hello Serial! \r\n");
		OSTimeDly(100,OS_OPT_TIME_DLY,&err);
//    LED1( ON );
//    OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//��ʱ����100ms
//    LED1( OFF);     
   // OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* 
 * ��������Task_LED2
 * ����  : 	LED����2��
 *			    ���ȼ�Ϊ5��
 *		      ��200ms�ļ���������ر�LED2
 * ����  ����
 * ���  : ��
 */
void Task_LED2(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;                	

  while (1)
  {
//    LED2( ON );
//    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);	 //��ʱ����200ms
//    LED2( OFF);
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* 
 * ��������Task_LED3
 * ����  : LED����3��
 *			   ���ȼ�Ϊ6��
 *		     ��300ms�ļ���������ر�LED3
 * ����  ����
 * ���  : ��
 */
void Task_LED3(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;      

  while (1)
  {
//    LED3( ON );
//    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);	 //��ʱ����300ms
//    LED3( OFF);
    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

void Task_GUI(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;		
	
	//GUIDEMO_Main();
	//��ʱ
	while(1)
	{
		OSTimeDly(1,OS_OPT_TIME_DLY,&err);
		}
}

/* -------------------------------------end of file------------------------------------ */
