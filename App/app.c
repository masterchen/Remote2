#include "app.h"
#include "bsp_touch.h"
#include "string.h"
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
	{Key_SpeedAdj,	        0x1b,0xff}, //22
	{Key_LegCntSwitch,	    0x0d,0xff}, //8
	{Key_BackUp,	        0x12,0x11}, //13
	{Key_BackDown,	        0x14,0x12}, //15
	{Key_WaistUp,	        0x0f,0x26}, //10
	{Key_WaistDown,	        0x11,0x27}, //12
	{Key_LegUp,	            0x0c,0x13}, //7
	{Key_LegDown,	        0x0e,0x14}, //9
	{Key_Trendelenburg,	    0x15,0x17}, //16
	{Key_AntiTrendelenburg,	0x18,0x21}, //19
	{key_Up,	            0x1a,0x08}, //21
	{Key_Down,	            0x16,0x09}, //17
	{Key_LeftLean,	        0x15,0x15}, //16
	{Key_RightLean,	        0x17,0x16}, //18
	{Key_HeadForward,	    0x13,0x22}, //14
	{Key_FootForward,	    0x10,0x23}, //11
	{Key_ReverseFunc,	    0x0a,0x00}, //5
	{Key_UnLock,	        0x09,0x02}, //4
	{Key_Lock,	            0x0b,0x01}, //6
	{Key_Start,	            0x1,0xff},
	{Key_Stop,	            0x1,0xff},
	{Key_Reset,	            0x1,0xff},
	{Key_Memo,	            0x03,0xff}, //23
	{Key_PageUp,	        0x05,0xff}, //25
	{Key_PageDown,	        0x06,0xff}, //26
	{Key_Func1,	            0x04,0xff}, //24
	{Key_Func2,	            0x07,0xff}, //27
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
unsigned char ReadBackData[25] = {0};
uint8_t logo_bmp[21632];
#define KEYMSG_Q_NUM   1
//#define DATAMSG_Q_NUM  4
OS_Q KEY_Msg;

#define COM2_TIMEOUT   250 //50ms

u8 CheckData[8]={0x00,0x03,0x00,0x01,0x00,0x14,0x15,0xD4};
unsigned char ReadBack[5]={0x01,0x02};
unsigned char buffera[4];

void ConvertUncharToChar(char *str,unsigned char* UnChar,int uclen)
{

	int i=0;
	for(i=0;i<uclen;i++)
	{
		sprintf(str+i*2,"%02X",UnChar[i]);
	}

}

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
		
		//SPI_Flash_Read(logo_bmp,0xC0020000,22030);		
		//OSTimeDly(500,OS_OPT_TIME_DLY,&err);	
		//SPI_Flash_Read(logo_bmp,0xC0030000,45968);	
		//OSTimeDly(500,OS_OPT_TIME_DLY,&err);	
		//LCD_DrawBMP(40,40,200,112,logo_bmp);
		 //GUI_Demo();
		 


		
	/*	
			UG_FillScreen(C_BLACK);
		  UG_WindowCreate(&window_1,obj_buff_wnd_1,MAX_OBJECTS,window_1_callback);
		
	*/
			//LCD_Clear(CYAN);
			OSTimeDly(500,OS_OPT_TIME_DLY,&err);
			//GPIO_SetBits(GPIOE,GPIO_Pin_6);
			//GPIO_SetBits(GPIOE,GPIO_Pin_2);
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
				OSQPost((OS_Q*	   )&KEY_Msg,
					   (void*	   )&key,
							 (OS_MSG_SIZE)1,
							 (OS_OPT	 )OS_OPT_POST_FIFO,
							 (OS_ERR*	 )&err);
				
			}
		*/
		  //printf ("the key = %d\r\n", key);
	
			//LCD_ShowString(80,100,300,16,16,(u8 *)key);
			//GUI_TOUCH_Exec(); 									//10ms一次GUI调用处理触屏
			//TOUCH_Scan(); 										//10ms一次GUI调用处理触屏
		{ 
			//LCD_Fill(10,30,150,170,RED);
			//LCD_Fill(200,30,250,170,BROWN);
	
			//OSTimeDly(100,OS_OPT_TIME_DLY,&err);
		}	
			//SPI_Flash_Read(wakeup_readback,0xC0000000,21632); 	 
		  //LCD9341_Drawpic(50,100,104,104,wakeup_readback);
			//printf("Hello Serial! \r\n");
			//OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	//	  LED1( ON );
	//	  OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);	//延时阻塞100ms
	//	  LED1( OFF);	  
	   // OSTimeDlyHMSM(0, 0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);

  }


/*			   
	tmr1=OSTmrCreate(OS_TMR * p_tmr, CPU_CHAR * p_name, 50, 10,OS_TMR_CALLBACK_PTR , OS_TMR_CALLBACK_PTR p_callback, void * p_callback_arg, OS_ERR * p_err)  


    tmr1=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);
    OS_TMR *OSTmrCreate( OS_TICK dly, OS_TICK period, OS_OPT opt, OS_TMR_CALLBACK_PTR p_callback, void * p_callback_arg,CPU_CHAR * p_name, OS_ERR * p_err)
*/

void Task_Key(void *p_arg)
{
    u8 key;

    OS_ERR err;
	
	OS_TMR first_tmr;
	
	static u8 lastkeyvalue=0;
	(void)p_arg; 

	OSTmrCreate((OS_TMR 			 *) &first_tmr,
			 	(CPU_CHAR			 *)"first tmr",
			 	(OS_TICK			  )10,
				(OS_TICK			  )10,
				(OS_OPT			      )OS_OPT_TMR_PERIODIC,
				(OS_TMR_CALLBACK_PTR  )tmr1_callback,
				(void				 *)0,
				(OS_ERR			     *)&err);

	OSTmrStart((OS_TMR * )&first_tmr,
				(OS_ERR * )&err);
	
	while (1)
	 {
		key=Key_Scan();

	    if(lastkeyvalue!=key)
		{	
		
		//if(key)
		//{
		   //LCD_ShowString(120,200,200,16,16,"got key");
		   OSQPost((OS_Q*	  )&KEY_Msg,
					(void*	  )&key,
					(OS_MSG_SIZE)1,
					(OS_OPT 	)OS_OPT_POST_FIFO,
					(OS_ERR*	)&err);

		lastkeyvalue=key;			
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
	ConvertUncharToChar(buffera,ReadBack,4);
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


void tmr1_callback(OS_TMR * ptmr, void * p_arg)
{
	OS_ERR err;
	s16 timewait = COM2_TIMEOUT;
	u8 size = 0;
	static u8 cnt=0;
	if(cnt==5)
		{	
		  cnt=0;
		  comSendBuf(COM2, CheckData, 8);
		  while(timewait >0){
			if(comGetChar(COM2, &ReadBackData[size]) == 1){
				size++;
				printf("Got reply: 0x%x",ReadBackData[size]);
				if(size == 25)
					break;
			}
			OSTimeDlyHMSM(0, 0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
			timewait -= 10;
		}
	cnt++;
 }

}



/* -------------------------------------end of file------------------------------------ */
