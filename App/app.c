#include "app.h"
#include "bsp_touch.h"
void Task_GUI(void *p_arg);
extern void TOUCH_Scan(void);

extern	OS_TCB	StartUp_TCB;					

static  OS_TCB	LED1_TCB;		   				
static	CPU_STK	LED1_Stk[TASK_LED1_STK_SIZE];	   

static  OS_TCB	KEY_TCB;		   					
static	CPU_STK	KEY_Stk[TASK_KEY_STK_SIZE];


static  OS_TCB	GUI_TCB;		   					
static	CPU_STK	GUI_Stk[TASK_GUI_STK_SIZE];

const sKeyMap KeyMap[] = {
	{Key_SpeedAdj,	        0x01,0xff},
	{Key_LegCntSwitch,	    0x01,0xff},
	{Key_BackUp,	        0x01,0x24},
	{Key_BackDown,	        0x01,0x15},
	{Key_WaistUp,	        0x01,0x27},
	{Key_WaistDown,	        0x01,0x26},
	{Key_LegUp,	            0x1,0x13},
	{Key_LegDown,	        0x1,0x14},
	{Key_Trendelenburg,	    0x1,0x17},
	{Key_AntiTrendelenburg,	0x1,0x21},
	{key_Up,	            0x1,0x08},
	{Key_Down,	            0x1,0x09},
	{Key_LeftLean,	        0x1,0x15},
	{Key_RightLean,	        0x1,0x16},
	{Key_HeadForward,	    0x1,0x22},
	{Key_FootForward,	    0x1,0x21},
	{Key_ReverseFunc,	    0x1,0x00},
	{Key_UnLock,	        0x1,0x02},
	{Key_Lock,	            0x1,0x01},
	{Key_Start,	            0x1,0xff},
	{Key_Stop,	            0x1,0xff},
	{Key_Reset,	            0x1,0xff},
	{Key_Memo,	            0x1,0xff},
	{Key_PageUp,	        0x1,0xff},
	{Key_PageDown,	        0x1,0xff},
	{Key_Func1,	            0x04,0xff},
	{Key_Func2,	            0x07,0xff},
	{key_ReadData,	        0xEA,0x14},// 用于读取数据
	{Key_Max,	            0xff,0xff},
};



const sKeyMap2 KeyMap2[] = {
	{Key_SpeedAdj,	        0x01},
	{Key_LegCntSwitch,	    0x01},
	{Key_BackUp,	        0x01},
	{Key_BackDown,	        0x01},
	{Key_WaistUp,	        0x01},
	{Key_WaistDown,	        0x01},
	{Key_LegUp,	            0x1},
	{Key_LegDown,	        0x1},
	{Key_Trendelenburg,	    0x1},
	{Key_AntiTrendelenburg,	0x1},
	{key_Up,	            0x1},
	{Key_Down,	            0x1},
	{Key_LeftLean,	        0x1},
	{Key_RightLean,	        0x1},
	{Key_HeadForward,	    0x1},
	{Key_FootForward,	    0x1},
	{Key_ReverseFunc,	    0x1},
	{Key_UnLock,	        0x1},
	{Key_Lock,	            0x1},
	{Key_Start,	            0x1},
	{Key_Stop,	            0x1},
	{Key_Reset,	            0x1},
	{Key_Memo,	            0x1},
	{Key_PageUp,	        0x1},
	{Key_PageDown,	        0x1},
	{Key_Func1,	            0x4},
	{Key_Func2,	            0x7},
	{key_ReadData,	        0xEA},// 用于读取数据
	{Key_Max,	            0xff},
};

#define KEYMAP_MAX_SIZE   (sizeof(KeyMap)/sizeof(sKeyMap))	


//u8 test[8];
uint8_t logo_bmp[21632];
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


	//创建任务KEY 扫描
	OSTaskCreate((OS_TCB	 *)&KEY_TCB,								// 任务控制块指针		   
			   (CPU_CHAR   *)"KEY",									// 任务名称
			   (OS_TASK_PTR )Task_Key, 							// 任务代码指针
			   (void	   *)0, 										// 传递给任务的参数parg
			   (OS_PRIO 	)TASK_KEY_PRIO,						// 任务优先级
			   (CPU_STK    *)&KEY_Stk[0],						  // 任务堆栈基地址
			   (CPU_STK_SIZE)TASK_KEY_STK_SIZE/10, 		  // 堆栈剩余警戒线
			   (CPU_STK_SIZE)TASK_KEY_STK_SIZE,				  // 堆栈大小
			   (OS_MSG_QTY	)0, 										// 可接收的最大消息队列数
			   (OS_TICK 	)0, 										// 时间片轮转时间
			   (void	   *)0, 										// 任务控制块扩展信息
			   (OS_OPT		)(OS_OPT_TASK_STK_CHK | 
							  OS_OPT_TASK_STK_CLR), 		// 任务选项
			   (OS_ERR	   *)&err); 	


							 
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
	//int k;
	//char temp[10]={1,2,3,4,5,6,7,8,9,10};
	//char *test[8]={"a","b","c","d","e","f","g","h"};
	//char dispbuf[1];
	u8 key;
    OS_ERR err;
    (void)p_arg;  
	SPI_Flash_Read(logo_bmp,0xC0000000,21632);	
    OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	 //LCD_DrawBMP(50,50,154,154,logo_bmp);
	 //GUI_Demo();

	
/*	
		UG_FillScreen(C_BLACK);
	  UG_WindowCreate(&window_1,obj_buff_wnd_1,MAX_OBJECTS,window_1_callback);
	
*/
		//LCD_Clear(CYAN);
		OSTimeDly(500,OS_OPT_TIME_DLY,&err);
		//GPIO_SetBits(GPIOE,GPIO_Pin_6);
		GPIO_SetBits(GPIOE,GPIO_Pin_2);
		//GPIO_SetBits(GPIOE,GPIO_Pin_3);
		
		/*
		LCD_DrawLine(30,20,200,200);
		LCD_DrawRectangle(0,0,50,50);
        LCD_FillColorRect(50,50,150,150,GREEN);
        LCD_DrawColorLine(0,0,320,240,BROWN);		
		LCD_DrawHLine(0,0,200,RED);
		LCD_FillColorRect(20,20,200,200,LCD_Color2Index_565(C_BROWN));
		LCD_ShowString(120,200,200,16,16,"Show me1 !");
	  */
  //while(1)
		{
			//for(k=0;k<8;k++)
			{
			  //LCD_ShowString(70,30,300,16,16,&USART_RX_BUF[2]);
				//LCD_ShowxNum(172,150,USART_RX_BUF[2],3,16,0X20);
				//LCD_ShowxNum(172,150,test[1],3,16,0X20);
				//if(USART_RX_BUF[1])
				{
					//LCD_ShowxNum(12,150,USART_RX_BUF[1],20,16,0X20);
				}
				//OSTimeDly(500,OS_OPT_TIME_DLY,&err);
				//LCD_ShowxNum(12,150,USART_RX_BUF[3],20,16,0X20);
				//OSTimeDly(500,OS_OPT_TIME_DLY,&err);
				//OSTimeDly(500,OS_OPT_TIME_DLY,&err);
			}
		}
		
	
/*
while(1)
{
	for(k=1;k<8;k++)
	{
		sprintf(dispbuf,"%d",test[k]);
		//LCD_ShowString(30,70,200,16,16,dispbuf);
		LCD_ShowChar(30,150,dispbuf,16,1);
		
	}
}		
*/		
		//GUI_Demo();
/*
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
	*/
	  //printf ("the key = %d\r\n", key);

		//LCD_ShowString(80,100,300,16,16,(u8 *)key);
		//GUI_TOUCH_Exec();										//10ms一次GUI调用处理触屏
		//TOUCH_Scan();											//10ms一次GUI调用处理触屏
	{ 
		//LCD_Fill(10,30,150,170,RED);
		//LCD_Fill(200,30,250,170,BROWN);

		//OSTimeDly(100,OS_OPT_TIME_DLY,&err);
	}	
		//SPI_Flash_Read(wakeup_readback,0xC0000000,21632); 	 
	  //LCD9341_Drawpic(50,100,104,104,wakeup_readback);
		//printf("Hello Serial! \r\n");
		//OSTimeDly(500,OS_OPT_TIME_DLY,&err);
//    LED1( ON );
//    OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//延时阻塞100ms
//    LED1( OFF);     
   // OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
  }




void Task_Key(void *p_arg)
{
    u8 key;

  OS_ERR err;
	(void)p_arg; 
	while (1)
	 {
	
		key=Key_Scan();
		if(key)
		{
		   LCD_ShowString(120,200,200,16,16,"got key");
		   OSQPost((OS_Q*	  )&KEY_Msg,
					(void*	  )&key,
					(OS_MSG_SIZE)1,
					(OS_OPT 	)OS_OPT_POST_FIFO,
					(OS_ERR*	)&err);
		}
		OSTimeDlyHMSM(0, 0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
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
{
	 
	u8* key;
	int n = 0;
	OS_MSG_SIZE size;

	OS_ERR err;
    (void)p_arg;
	UGUI_WindowInit();

	while (1)
	{		
		key=OSQPend((OS_Q*          )&KEY_Msg,
		           (OS_TICK         )0,
		           (OS_OPT          )OS_OPT_PEND_NON_BLOCKING,
		           (OS_MSG_SIZE*    )&size,
							 (CPU_TS*         )0,
		           (OS_ERR*         )&err);
		if(key != NULL)
		{
			for(n =0; n<KEYMAP_MAX_SIZE; n++){
				if(KeyMap[n].KeyCode == *key){
					
					break;
				}
			}
			UGUI_WindowStateHandler(KeyMap[n].KeyType);
			appCOM_SendCmdCode(KeyMap[n].CmdCode);
		} 
		else
		{
			OSTimeDlyHMSM(0, 0,0,30,OS_OPT_TIME_HMSM_STRICT,&err);
		}

		
	}
}

/* -------------------------------------end of file------------------------------------ */
