#include "app.h"
#include "bsp_touch.h"

void Task_GUI(void *p_arg);
extern void TOUCH_Scan(void);

extern	OS_TCB	StartUp_TCB;					

static  OS_TCB	LED1_TCB;		   				
static	CPU_STK	LED1_Stk[TASK_LED1_STK_SIZE];	   

static  OS_TCB	GUI_TCB;		   					
static	CPU_STK	GUI_Stk[TASK_GUI_STK_SIZE];

u8 wakeup_readback[21632] = {0};	 
u8 tmpData[20] = {0};
#define KEYMSG_Q_NUM   1
//#define DATAMSG_Q_NUM  4
OS_Q KEY_Msg;
//OS_Q DATA_Msg;

/* 
 * 函数名：Task_Start
 * 描述  : 启动任务，
 *			   优先级为3，
 *		     创建LED1、LED2和LED3的任务
 * 输入  ：无
 * 输出  : 无
 */
void Task_Start(void *p_arg)
{
	OS_ERR err;
  (void)p_arg;	
	
  OSQCreate((OS_Q*        )&KEY_Msg,
						(CPU_CHAR*    )"KEY Msg",
						(OS_MSG_QTY   )KEYMSG_Q_NUM,
						(OS_ERR*      )&err);
/*	
  OSQCreate((OS_Q*        )&DATA_Msg,
						(CPU_CHAR*    )"DATA Msg",
						(OS_MSG_QTY   )DATAMSG_Q_NUM,
						(OS_ERR*      )&err);
*/	
	//创建任务LED1	
	OSTaskCreate((OS_TCB     *)&LED1_TCB,					            // 任务控制块指针          
               (CPU_CHAR   *)"LED1",						            // 任务名称
               (OS_TASK_PTR )Task_LED1,					            // 任务代码指针
               (void       *)0,							                // 传递给任务的参数parg
               (OS_PRIO     )TASK_LED1_PRIO,				        // 任务优先级
               (CPU_STK    *)&LED1_Stk[0],				          // 任务堆栈基地址
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE/10,		      // 堆栈剩余警戒线
               (CPU_STK_SIZE)TASK_LED1_STK_SIZE,			      // 堆栈大小
               (OS_MSG_QTY  )0,							                // 可接收的最大消息队列数
               (OS_TICK     )0,							                // 时间片轮转时间
               (void       *)0,							                // 任务控制块扩展信息
               (OS_OPT      )(OS_OPT_TASK_STK_CHK | 
                              OS_OPT_TASK_STK_CLR),	        // 任务选项
               (OS_ERR     *)&err);						              // 返回值
							 
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
							 
               
  //任务删除自己	
	OSTaskDel(&StartUp_TCB,&err);							 
}


/*
extern  u8 RX_buffer[tbuf];
extern u8 RX_num;				 //接收计数变量
u8 Data_compare(u8 *p)
{ 
	if(strstr(RX_buffer,p)!=NULL)
	    return 1;
	else
		return 0;
}
*/

/* 
 * 函数名：Task_LED1
 * 描述  : LED任务1，
 *		     优先级为4，
 *		     以100ms的间隔点亮、关闭LED1
 * 输入  ：无
 * 输出  : 无
 */
void Task_LED1(void *p_arg)
{
	 u8 key;
  OS_ERR err;
  (void)p_arg;  
/*	
	  tmpData[0] = '0';
		tmpData[1] = '0';
	  tmpData[3] = '0';
		tmpData[4] = '0';
*/		
	
		//GPIO_SetBits(GPIOE,GPIO_Pin_6);
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		GPIO_SetBits(GPIOE,GPIO_Pin_3);
  while (1)
  {

		key=Key_Scan();
		if(key)
		{
			OSQPost((OS_Q*     )&KEY_Msg,
			       (void*      )&key,
						 (OS_MSG_SIZE)1,
						 (OS_OPT     )OS_OPT_POST_FIFO,
						 (OS_ERR*    )&err);
			
		}
		
	  //printf ("the key = %d\r\n", key);

		//LCD_ShowString(80,100,300,16,16,(u8 *)key);
		//GUI_TOUCH_Exec();										//10ms一次GUI调用处理触屏
		//TOUCH_Scan();											//10ms一次GUI调用处理触屏
	{ 
		LCD_ShowString(120,200,200,16,16,"Show me1 !");
		LCD_Fill(10,30,150,170,RED);
		//LCD_Fill(200,30,250,170,BROWN);

		//OSTimeDly(100,OS_OPT_TIME_DLY,&err);
	}	
		//SPI_Flash_Read(wakeup_readback,0xC0000000,21632); 	 
	  //LCD9341_Drawpic(50,100,104,104,wakeup_readback);
		//printf("Hello Serial! \r\n");
		OSTimeDly(500,OS_OPT_TIME_DLY,&err);
//    LED1( ON );
//    OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//延时阻塞100ms
//    LED1( OFF);     
   // OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* 
 * 函数名：Task_LED2
 * 描述  : 	LED任务2，
 *			    优先级为5，
 *		      以200ms的间隔点亮、关闭LED2
 * 输入  ：无
 * 输出  : 无
 */
void Task_LED2(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;                	

	
//    LED2( ON );
//    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);	 //延时阻塞200ms
//    LED2( OFF);
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
  
}

/* 
 * 函数名：Task_LED3
 * 描述  : LED任务3，
 *			   优先级为6，
 *		     以300ms的间隔点亮、关闭LED3
 * 输入  ：无
 * 输出  : 无
 */
void Task_LED3(void *p_arg)
{
  OS_ERR err;
  (void)p_arg;      

  while (1)
  {
//    LED3( ON );
//    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);	 //延时阻塞300ms
//    LED3( OFF);
    OSTimeDlyHMSM(0, 0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

void Task_GUI(void *p_arg)

/*{
  OS_ERR err;
  (void)p_arg;		

	
	//GUIDEMO_Main();
	//延时
	while(1)
	{
		OSTimeDly(1,OS_OPT_TIME_DLY,&err);
		}
}
*/

{
	u8 *key;
	OS_MSG_SIZE size;
	
  OS_ERR err;
  (void)p_arg;   
	
	LCD_ShowString(120,100,200,16,16,"task two here is !");

  while (1)
  {
	
		key=OSQPend((OS_Q*          )&KEY_Msg,
		           (OS_TICK         )0,
		           (OS_OPT          )OS_OPT_PEND_BLOCKING,
		           (OS_MSG_SIZE*    )&size,
							 (CPU_TS*         )0,
		           (OS_ERR*         )&err);
	
		switch(*key)
		{
			case 3:
				//GPIO_SetBits(GPIOE,GPIO_Pin_6);
				SPI_Flash_Read(wakeup_readback,0xC0000000,21632); 	 
				Gui_Drawbmp16WH(50,100,104,104,wakeup_readback);
				Uart1SendStr( "\r\n03 03 03 03 03 03 03 03\n");
				break;
			case 4:		
				GPIO_ResetBits(GPIOE,GPIO_Pin_6);
				Uart1SendStr( "\r\n04 04 04 04 04 04 04 04\n");
				break;
			case 5:
				Uart1SendStr( "\r\n05 05 05 05 05 05 05 05\n");
				break;
			case 6:
				Uart1SendStr( "\r\n06 06 06 06 06 06 06 06\n");
				break;
			case 7:			
				Uart1SendStr( "\r\n07 07 07 07 07 07 07 07\n");
				break;
			case 8:			
				Uart1SendStr( "\r\n08 08 08 08 08 08 08 08\n");
				break;
			case 9:			
				Uart1SendStr( "\r\n09 09 09 09 09 09 09 09\n");
				break;
			case 10:		
				Uart1SendStr( "\r\n10 10 10 10 10 10 10 10\n");
				break;
			case 11:			
				Uart1SendStr( "\r\n11 11 11 11 11 11 11 11\n");
				break;
			case 12:			
				Uart1SendStr( "\r\n12 12 12 12 12 12 12 12\n");
				break;
			case 13:			
				Uart1SendStr( "\r\n13 13 13 13 13 13 13 13\n");
				break;
			case 14:			
				Uart1SendStr( "\r\n14 14 14 14 14 14 14 14\n");
				break;
			case 15:			
				Uart1SendStr( "\r\n15 07 07 07 07 07 07 07\n");
				break;
			case 16:		
				Uart1SendStr( "\r\n16 07 07 07 07 07 07 07\n");
				break;
			case 17:			
				Uart1SendStr( "\r\n17 07 07 07 07 07 07 07\n");
				break;
			case 18:			
				Uart1SendStr( "\r\n18 07 07 07 07 07 07 07\n");
				break;
			case 19:			
				Uart1SendStr( "\r\n19 07 07 07 07 07 07 07\n");
				break;
			case 20:			
				Uart1SendStr( "\r\n20 07 07 07 07 07 07 07\n");
				break;
			case 21:		
				Uart1SendStr( "\r\n21 07 07 07 07 07 07 07\n");
				break;
			case 22:			
				Uart1SendStr( "\r\n22 07 07 07 07 07 07 07\n");
				break;
			case 23:			
				Uart1SendStr( "\r\n23 07 07 07 07 07 07 07\n");
				break;
			case 24:		
				Uart1SendStr( "\r\n24 07 07 07 07 07 07 07\n");
				break;
			case 25:		
				Uart1SendStr( "\r\n25 07 07 07 07 07 07 07\n");
				break;
			case 26:		
				Uart1SendStr( "\r\n26 07 07 07 07 07 07 07\n");
				break;
			case 27:			
				Uart1SendStr( "\r\n27 07 07 07 07 07 07 07\n");
				break;
		}
	
//    LED2( ON );
//    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);	 //延时阻塞200ms
//    LED2( OFF);
    OSTimeDlyHMSM(0, 0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
  }
}

/* -------------------------------------end of file------------------------------------ */
