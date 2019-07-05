#include "app.h"
#include "image.h"
#include "delay.h"
#include "string.h"

#define led1on GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_2)
#define led2on GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3)

extern unsigned char ReadBackData[25];
extern unsigned char ReadBack;
extern unsigned char buffera[4];

typedef struct{
	unsigned char KeyStateIndex;
	unsigned char KeyEnterState;
	unsigned char KeyBackState;
	unsigned char KeyLeftState;
	unsigned char KeyRightState;
	unsigned char KeySetState;
	void (*CurrentOperate)();
}KbdTabStruct;

KbdTabStruct const MainKeyTab[55]={

//index,leftsur,left,right,rightsur,mix
{0,1,2,3,4,5,(*fastmem_select_mem_window)},
{1,2,3,4,5,0,(*fastmem_select_exe_window)},
{2,3,4,5,0,1,(*UGUI_ShowSubWindow)},
{3,4,5,0,1,2,(*UGUI_ShowMainWindow)},
{4,5,0,1,2,3,(*UGUI_ShowSpasha)},
{5,0,1,2,3,4,(*UGUI_ShowSpash)},
};

//unsigned char buffer[8]={1,2,3};
//"1234"

void (*KeyFuncPtr)();


extern UG_RESULT _HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
extern UG_RESULT _HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
extern void _HW_DrawPoint(UG_S16 x, UG_S16 y, UG_COLOR c);
/* GUI structure */
UG_GUI gui;
#define MAX_OBJECTS        10
/* Window 1 */
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button1_1;
UG_BUTTON button1_2;
UG_BUTTON button1_3;
UG_BUTTON button1_4;
UG_BUTTON button1_5;
UG_BUTTON button1_6;

/* Window 2 */
UG_WINDOW window_2;
UG_OBJECT obj_buff_wnd_2[MAX_OBJECTS];
UG_BUTTON button2_1;
UG_TEXTBOX textbox2_1;
UG_TEXTBOX textbox2_2;
UG_TEXTBOX textbox2_3;
UG_IMAGE image2_1;
UG_IMAGE image2_2;


/* Window 3 */
UG_WINDOW window_3;
UG_OBJECT obj_buff_wnd_3[MAX_OBJECTS];
UG_BUTTON button3_1;
UG_TEXTBOX textbox3_1;

/*fastmem_select_exe*/

UG_WINDOW fastmem_select_exe;
UG_OBJECT obj_buff_fse_1[MAX_OBJECTS];
UG_BUTTON button_fse_1;
UG_BUTTON button_fse_2;

/*fastmem_select_mem*/

UG_WINDOW fastmem_select_mem;
UG_OBJECT obj_buff_fsm_1[MAX_OBJECTS];
UG_BUTTON button_fsm_1;
UG_BUTTON button_fsm_2;



/* FSM */
#define STATE_MAIN_MENU                0
#define STATE_BENCHMARK_RUN            1
#define STATE_BENCHMARK_RESULT         2

volatile UG_U32 state;
volatile UG_U32 next_state;

/* Benchmark */
volatile UG_U32 timer;
volatile UG_U32 hw_acc = 1;
char result_str[30];
UG_S16 xs,ys;
UG_S16 xe,ye;
UG_COLOR c;



typedef struct S_WindowPageInfo{
	char * winName;
	UG_WINDOW winHandler;
	eKeyType  acceptKey[Key_Max];
}WindowPageInfo;


typedef struct S_WindowStateInfo{
	WindowPageInfo curPage;
	WindowPageInfo prePage;
	WindowPageInfo nextPage;
}WindowStateInfo;

/*
WindowPageInfo window1_1={"start_window",&window1,Key_Func2};
	
WindowStateInfo fastmem_select_exe={fastmem_select_exe,main_window,fastmem_confirm_exe};
WindowStateInfo fastmem_select_mem={fastmem_select_mem,fastmem_select_exe,fastmem_confirm_mem}
WindowStateInfo fastmem_confirm_exe={fastmem_confirm_exe,fastmem_select_exe,fastmem_exe,};
WindowStateInfo fastmem_confirm_mem={fastmem_confirm_mem,fastmem_select_mem,fastmem_mem,};
WindowStateInfo fastmem_exe={fastmem_exe,fastmem_confirm_exe,main_window};
WindowStateInfo fastmem_mem={fastmem_mem,fastmem_confirm_mem,main_window};
*/






void fastmem_select_mem_callback( UG_MESSAGE* msg )
{
  
}
void fastmem_select_exe_callback( UG_MESSAGE* msg )
{
 
}








/* Callback function for the main menu */
void window_1_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            case BTN_ID_0: /* Toggle green LED */
            {
               GPIO_SetBits(GPIOD , GPIO_Pin_9);
               break;
            }
            case BTN_ID_1: /* Toggle red LED */
            {
               GPIO_ResetBits(GPIOD , GPIO_Pin_9);
               break;
            }
            case BTN_ID_2: /* Show ?UI info */
            {
               UG_WindowShow( &window_2 );
               break;
            }
            case BTN_ID_3: /* Toggle hardware acceleration */
            {
               if ( !hw_acc )
               {
                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_RED );
                  UG_ButtonSetText( &window_1, BTN_ID_3, "HW_ACC\nOFF" );
                  UG_DriverEnable( DRIVER_DRAW_LINE );
                  UG_DriverEnable( DRIVER_FILL_FRAME );
               }
               else
               {
                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_BLUE );
                  UG_ButtonSetText( &window_1, BTN_ID_3, "HW_ACC\nON" );
                  UG_DriverDisable( DRIVER_DRAW_LINE );
                  UG_DriverDisable( DRIVER_FILL_FRAME );
               }
               hw_acc = !hw_acc;
               break;
            }
            case BTN_ID_4: /* Start benchmark */
            {
               next_state = STATE_BENCHMARK_RUN;
               break;
            }
            case BTN_ID_5: /* Resize window */
            {
               static UG_U32 tog;

               if ( !tog )
               {
                  UG_WindowResize( &window_1, 0, 40, 239, 319-40 );
               }
               else
               {
                  UG_WindowResize( &window_1, 0, 0, 239, 319 );
               }
               tog = ! tog;
               break;
            }
         }
      }
   }
}


/* Callback function for the info window */
void window_2_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            case BTN_ID_0:
            {
               UG_WindowHide( &window_2 );
               break;
            }
         }
      }
   }
}

/* Callback function for the result window */
void window_3_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            /* OK button */
            case BTN_ID_0:
            {
               UG_WindowShow( &window_1 );
               break;
            }
         }
      }
   }
}


/* -------------------------------------------------------------------------------- */
/* init GUI window											*/
/* -------------------------------------------------------------------------------- */

void UGUI_WindowInit(void)
{
	 UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))_HW_DrawPoint,320,240);

	 /* Register hardware acceleration */
	 UG_DriverRegister( DRIVER_DRAW_LINE, (void*)_HW_DrawLine );
	 UG_DriverRegister( DRIVER_FILL_FRAME, (void*)_HW_FillFrame );
	 UG_DriverEnable( DRIVER_DRAW_LINE );
	 UG_DriverEnable( DRIVER_FILL_FRAME );
	 UG_FillScreen( C_BLACK );
}

void UGUI_WindowDemo(void)
{
	 /* Create Window 1 */
	 UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback );
	 //UG_WindowSetTitleText( &window_1, "GUI @ STM32F103" );
	 UG_WindowSetTitleTextFont( &window_1, &FONT_12X20 );
	 /* Create some Buttons */
	 UG_ButtonCreate( &window_1, &button1_1, BTN_ID_0, 10, 10, 110, 60 );
	 UG_ButtonCreate( &window_1, &button1_2, BTN_ID_1, 10, 80, 110, 130 );
	 UG_ButtonCreate( &window_1, &button1_3, BTN_ID_2, 10, 150, 110,200 );
	 UG_ButtonCreate( &window_1, &button1_4, BTN_ID_3, 120, 10, UG_WindowGetInnerWidth( &window_1 ) - 10 , 60 );
	 UG_ButtonCreate( &window_1, &button1_5, BTN_ID_4, 120, 80, UG_WindowGetInnerWidth( &window_1 ) - 10, 130 );
	 UG_ButtonCreate( &window_1, &button1_6, BTN_ID_5, 120, 150, UG_WindowGetInnerWidth( &window_1 ) - 10, 200 );
	 /* Configure Button 1 */
	 UG_ButtonSetFont( &window_1, BTN_ID_0, &FONT_12X20 );
	 UG_ButtonSetBackColor( &window_1, BTN_ID_0, C_LIME );
	 UG_ButtonSetText( &window_1, BTN_ID_0, "LED\nON" );
	  /* Configure Button 2 */
	 UG_ButtonSetFont( &window_1, BTN_ID_1, &FONT_12X20 );
	 UG_ButtonSetBackColor( &window_1, BTN_ID_1, C_RED );
	  UG_ButtonSetForeColor( &window_1, BTN_ID_1,C_YELLOW);
	 UG_ButtonSetText( &window_1, BTN_ID_1, "LED\nOFF" );
	  /* Configure Button 3 */
	 UG_ButtonSetFont( &window_1, BTN_ID_2, &FONT_12X20 );
	 UG_ButtonSetText( &window_1, BTN_ID_2, "About\nGUI" );
	 UG_WindowShow( &window_1 );
	 
	 /* Configure Button 4 */
	 UG_ButtonSetFont( &window_1, BTN_ID_3, &FONT_12X20 );
	 UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_RED );
	 UG_ButtonSetText( &window_1, BTN_ID_3, "HW_ACC\nOFF" );
	 /* Configure Button 5 */
	 UG_ButtonSetFont( &window_1, BTN_ID_4, &FONT_8X14 );
	 UG_ButtonSetText( &window_1, BTN_ID_4, "Start\nBenchmark" );
	 /* Configure Button 6 */
	 UG_ButtonSetFont( &window_1, BTN_ID_5, &FONT_10X16 );
	 UG_ButtonSetText( &window_1, BTN_ID_5, "Resize\nWindow" );
	  /* -------------------------------------------------------------------------------- */
	 /* Create Window 2 (?UI Info)														*/
	 /* -------------------------------------------------------------------------------- */
	 UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
	 UG_WindowSetTitleText( &window_2, "Info" );
	 UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
	 UG_WindowResize( &window_2, 20, 40, 219, 279 );
	 /* Create Button 1 */
	 UG_ButtonCreate( &window_2, &button2_1, BTN_ID_0, 40, 150, UG_WindowGetInnerWidth( &window_2 )-40, 200 );
	 UG_ButtonSetFont( &window_2, BTN_ID_0, &FONT_22X36 );
	 UG_ButtonSetText( &window_2, BTN_ID_0, "OK!" );
	 /* Create Textbox 1 */
	 UG_TextboxCreate( &window_2, &textbox2_1, TXB_ID_0, 10, 10, UG_WindowGetInnerWidth( &window_2 )-10, 40 );
	 UG_TextboxSetFont( &window_2, TXB_ID_0, &FONT_16X26 );
	 UG_TextboxSetText( &window_2, TXB_ID_0, "GUI v0.3" );
	 UG_TextboxSetAlignment( &window_2, TXB_ID_0, ALIGN_TOP_CENTER );
	 /* Create Textbox 2 */
	 UG_TextboxCreate( &window_2, &textbox2_2, TXB_ID_1, 10, 125, UG_WindowGetInnerWidth( &window_2 )-10, 135 );
	 UG_TextboxSetFont( &window_2, TXB_ID_1, &FONT_6X8 );
	 UG_TextboxSetText( &window_2, TXB_ID_1, "www.embeddedlightning.com" );
	 UG_TextboxSetAlignment( &window_2, TXB_ID_1, ALIGN_BOTTOM_CENTER );
	 UG_TextboxSetForeColor( &window_2, TXB_ID_1, C_BLUE );
	 UG_TextboxSetHSpace( &window_2, TXB_ID_1, 1 );
	 /* Create Image 1 */
	 UG_ImageCreate( &window_2, &image2_1, IMG_ID_0, (UG_WindowGetInnerWidth( &window_2 )>>1) - (logo.width>>1), 40, 0, 0 );
	 UG_ImageSetBMP( &window_2, IMG_ID_0, &logo );
	
	
	 UG_TextboxCreate( &window_2, &textbox2_3, TXB_ID_2,104, 40, 200, 60 );
	 UG_TextboxSetFont( &window_2, TXB_ID_2, &FONT_8X14 );
	 UG_TextboxSetText( &window_2, TXB_ID_2, "11AA" );
	 UG_TextboxSetForeColor( &window_2, TXB_ID_2, C_YELLOW_GREEN );
	 UG_TextboxSetBackColor( &window_2, TXB_ID_2, C_BLACK);
	 UG_PutChar('a',10,40,C_BLACK,C_YELLOW_GREEN);
	 /* -------------------------------------------------------------------------------- */
	 /* Create Window 3 (Benchmark Result)												 */
	 /* -------------------------------------------------------------------------------- */
	 UG_WindowCreate( &window_3, obj_buff_wnd_3, MAX_OBJECTS, window_3_callback );
	 UG_WindowSetTitleText( &window_3, "Benchmark Result" );
	 UG_WindowSetTitleTextFont( &window_3, &FONT_10X16 );
	 UG_WindowResize( &window_3, 20, 90, 219, 230 );
	 /* Create Textbox 1 */
	 UG_TextboxCreate( &window_3, &textbox3_1, TXB_ID_0, 5, 10, UG_WindowGetInnerWidth( &window_3 )-5, 60 );
	 UG_TextboxSetFont( &window_3, TXB_ID_0, &FONT_12X20 );
	 UG_TextboxSetText( &window_3, TXB_ID_0, "Result:\n99999 frm/sec" );
	 UG_TextboxSetAlignment( &window_3, TXB_ID_0, ALIGN_TOP_CENTER );
	 /* Create Button 1 */
	 UG_ButtonCreate( &window_3, &button3_1, BTN_ID_0, 40, 65, UG_WindowGetInnerWidth( &window_3 )-40, 100 );
	 UG_ButtonSetFont( &window_3, BTN_ID_0, &FONT_12X16 );
	 UG_ButtonSetText( &window_3, BTN_ID_0, "OK!" );
	
	 /* -------------------------------------------------------------------------------- */
	 /* Start demo application															 */
	 /* -------------------------------------------------------------------------------- */
	 /* Show Window 1 */
	   delay_ms(1000);
	  while(1)  
	  {
		  UG_WindowShow( &window_1 ); 			  
		  delay_ms(2000);			  
		  UG_Update();	  //Windows刷新
		  UG_WindowShow( &window_2 );
		  UG_TouchUpdate(45,155,OBJ_TOUCH_STATE_IS_PRESSED);
		  delay_ms(3000);
		  UG_Update();	  //Windows刷新
		  UG_WindowShow( &window_3 );
		  delay_ms(1000);
		  UG_Update();  //Windows刷新
	  }

}
/* -------------------------------------------------------------------------------- */
/* Create SplashWindow										                   */
/* -------------------------------------------------------------------------------- */

void UGUI_ShowSpash(void)
{

	UG_Update();	//Windows??

	UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
	UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
	UG_WindowSetTitleColor( &window_2, C_WHITE );
	//UG_WindowResize( &window_2, 20, 40, 219, 279 );
	put_chinese_char16(220,5,0,C_BLACK);
	put_chinese_char16(250,5,1,C_BLACK);
	put_chinese_char16(280,5,2,C_BLACK);

	//put_chinese_char(250,150,0,C_BLACK);
	//put_chinese_char(210,75,1,C_BLACK);//威
	
	/* Create Textbox 1*/
	UG_TextboxCreate( &window_2, &textbox2_1, TXB_ID_0, 200, 50, UG_WindowGetInnerWidth( &window_2 )-10, 100 );
	UG_TextboxSetFont( &window_2, TXB_ID_0, &FONT_22X36 );
	//UG_TextboxSetText( &window_2, TXB_ID_0, "WEGO" );
	UG_TextboxSetText( &window_2, TXB_ID_0, buffera);
	//UG_TextboxSetAlignment( &window_2, TXB_ID_0, ALIGN_TOP_CENTER );
	UG_TextboxSetForeColor( &window_2, TXB_ID_0, C_BLACK );
	//_UG_UpdateObjects( &window_2 );

	//_HW_DrawLine(2,210,318,210,C_BLACK );

	//UG_ImageCreate( &window_2, &image2_2, IMG_ID_1, 40, 40, 144, 144);
	//UG_ImageSetBMP( &window_2, IMG_ID_1, &logo );		

	UG_WindowShow( &window_2 );	




}

void UGUI_ShowChineseWindow(void)
{

	/* Create Window 1 */
	UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback );
	UG_WindowSetTitleTextFont( &window_1, &FONT_12X20 );
	/* Create some Buttons */
	UG_ButtonCreate( &window_1, &button1_1, BTN_ID_0, 10, 10, 110, 60 );
	UG_ButtonCreate( &window_1, &button1_2, BTN_ID_1, 10, 80, 110, 130 );
	UG_ButtonCreate( &window_1, &button1_3, BTN_ID_2, 10, 150, 110,200 );
	UG_ButtonCreate( &window_1, &button1_4, BTN_ID_3, 120, 10, UG_WindowGetInnerWidth( &window_1 ) - 10 , 60 );
	UG_ButtonCreate( &window_1, &button1_5, BTN_ID_4, 120, 80, UG_WindowGetInnerWidth( &window_1 ) - 10, 130 );
	UG_ButtonCreate( &window_1, &button1_6, BTN_ID_5, 120, 150, UG_WindowGetInnerWidth( &window_1 ) - 10, 200 );
	/* Configure Button 1 */
	UG_ButtonSetChineseFont( &window_1, BTN_ID_0, &CN_FONT_32X32);
	UG_ButtonSetBackColor( &window_1, BTN_ID_0, C_LIME );
	UG_ButtonSetText( &window_1, BTN_ID_0, "威高" );
	 /* Configure Button 2 */
	UG_ButtonSetFont( &window_1, BTN_ID_1, &FONT_12X20 );
	UG_ButtonSetBackColor( &window_1, BTN_ID_1, C_RED );
	 UG_ButtonSetForeColor( &window_1, BTN_ID_1,C_YELLOW);
	UG_ButtonSetText( &window_1, BTN_ID_1, "LED\nOFF" );
	 /* Configure Button 3 */
	UG_ButtonSetFont( &window_1, BTN_ID_2, &FONT_12X20 );
	UG_ButtonSetText( &window_1, BTN_ID_2, "About\nGUI" );
	UG_WindowShow( &window_1 );

}



/*
void UGUI_ShowImage(UG_WINDOW* window, UG_BMP* bmp )
{
	UG_ImageCreate( &window_2, &image2_2, IMG_ID_1, 40, 40, 144, 144);
	UG_ImageSetBMP( &window_2, IMG_ID_1, &bmp );		
	UG_Update();	//Windows刷新

}
*/

void UGUI_ShowSpasha(void)
{
	UG_Update();	//Windows??
	
	UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
	UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
	UG_WindowSetTitleColor( &window_2, C_WHITE );
	
	UG_TextboxCreate( &window_2, &textbox2_2, TXB_ID_1, 200, 50, UG_WindowGetInnerWidth( &window_2 )-10, 100 );
	UG_TextboxSetFont( &window_2, TXB_ID_1, &FONT_22X36 );
	UG_TextboxSetText( &window_2, TXB_ID_1, &buffera[2]);
	UG_TextboxSetForeColor( &window_2, TXB_ID_1, C_BLACK );

	//_UG_UpdateObjects( &window_2 );

    //UG_ImageCreate( &window_2, &image2_1, IMG_ID_0, 40, 40, 144, 144 );
	//UG_ImageSetBMP( &window_2, IMG_ID_0, &logo );		

	//_UG_ImageUpdate(&window_2, obj_buff_wnd_2);

	UG_WindowShow( &window_2 );	

}

/* -------------------------------------------------------------------------------- */
/* Create  MainWindow										                   */
/* -------------------------------------------------------------------------------- */

void UGUI_ShowMainWindow(void)
{
	UG_Update();	//Windows??

	UG_WindowCreate( &window_3, obj_buff_wnd_3, MAX_OBJECTS, window_3_callback );
	UG_WindowSetTitleText( &window_3, "Benchmark Result" );
	UG_WindowSetTitleTextFont( &window_3, &FONT_10X16 );
	UG_WindowResize( &window_3, 20, 90, 219, 230 );
	/* Create Textbox 1 */
	UG_TextboxCreate( &window_3, &textbox3_1, TXB_ID_0, 5, 10, UG_WindowGetInnerWidth( &window_3 )-5, 60 );
	UG_TextboxSetFont( &window_3, TXB_ID_0, &FONT_12X20 );
	UG_TextboxSetText( &window_3, TXB_ID_0, "Result:\n99999 frm/sec" );
	UG_TextboxSetAlignment( &window_3, TXB_ID_0, ALIGN_TOP_CENTER );
	/* Create Button 1 */
	UG_ButtonCreate( &window_3, &button3_1, BTN_ID_0, 40, 65, UG_WindowGetInnerWidth( &window_3 )-40, 100 );
	UG_ButtonSetFont( &window_3, BTN_ID_0, &FONT_12X16 );
	UG_ButtonSetText( &window_3, BTN_ID_0, "OK!" );


	UG_WindowShow( &window_3 );							

}

void UGUI_ShowSubWindow(void)
{ 
	UG_Update();	//Windows??

	UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
	UG_WindowSetTitleText( &window_2, "Info" );
	UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
	UG_WindowResize( &window_2, 20, 40, 219, 279 );
	/* Create Button 1 */
	UG_ButtonCreate( &window_2, &button2_1, BTN_ID_0, 40, 150, UG_WindowGetInnerWidth( &window_2 )-40, 200 );
	UG_ButtonSetFont( &window_2, BTN_ID_0, &FONT_22X36 );
	UG_ButtonSetText( &window_2, BTN_ID_0, "OK!" );
	/* Create Textbox 1 */
	UG_TextboxCreate( &window_2, &textbox2_1, TXB_ID_0, 10, 10, UG_WindowGetInnerWidth( &window_2 )-10, 40 );
	UG_TextboxSetFont( &window_2, TXB_ID_0, &FONT_16X26 );
	UG_TextboxSetText( &window_2, TXB_ID_0, "GUI v0.3" );
	UG_TextboxSetAlignment( &window_2, TXB_ID_0, ALIGN_TOP_CENTER );
	/* Create Textbox 2
	UG_TextboxCreate( &window_2, &textbox2_2, TXB_ID_1, 10, 125, UG_WindowGetInnerWidth( &window_2 )-10, 135 );
	UG_TextboxSetFont( &window_2, TXB_ID_1, &FONT_6X8 );
	UG_TextboxSetText( &window_2, TXB_ID_1, "www.embeddedlightning.com" );
	UG_TextboxSetAlignment( &window_2, TXB_ID_1, ALIGN_BOTTOM_CENTER );
	UG_TextboxSetForeColor( &window_2, TXB_ID_1, C_BLUE );
	UG_TextboxSetHSpace( &window_2, TXB_ID_1, 1 );
	 */
	/* Create Image 1 */
    //UG_ImageCreate( &window_2, &image2_1, IMG_ID_0, 40, 40, 144, 144 );
	//UG_ImageSetBMP( &window_2, IMG_ID_0, &logo );		
	UG_WindowShow( &window_2 ); 						

}

void fastmem_select_exe_window(void)
{
	OS_ERR err;

	UG_Update();	//Windows??

	UG_WindowCreate( &fastmem_select_exe, obj_buff_fse_1, MAX_OBJECTS, fastmem_select_exe_callback );
	UG_WindowSetTitleTextFont( &fastmem_select_exe, &FONT_22X36 );
	UG_WindowSetTitleColor( &fastmem_select_exe, C_WHITE );
	UG_WindowResize( &fastmem_select_exe, 20, 40, 219, 279 );
	//_UG_UpdateObjects( &fastmem_select_exe );

	put_chinese_char32(220,5,7,C_RED);
	put_chinese_char32(280,5,8,C_RED);
	put_chinese_char32(120,5,11,C_RED);

	put_chinese_char32(80,200,9,C_RED);
	put_chinese_char32(120,200,5,C_RED);
	
	put_chinese_char32(160,200,6,C_RED);
	put_chinese_char32(200,200,10,C_RED);	
	
	UG_ButtonCreate( &fastmem_select_exe, &button_fse_1, BTN_ID_0, 20, 20, 140, 140 );

	UG_ButtonSetBackColor( &fastmem_select_exe, BTN_ID_0, C_WHITE);
	//_UG_UpdateObjects( &fastmem_select_exe );
	put_chinese_char80(60,80,0,C_BLACK);
	
	OSTimeDly(500,OS_OPT_TIME_DLY,&err);
	OSTimeDly(500,OS_OPT_TIME_DLY,&err);	
	
	UG_ButtonSetBackColor( &fastmem_select_exe, BTN_ID_0, C_BROWN);	
	//_UG_UpdateObjects( &fastmem_select_exe );
	put_chinese_char80(60,80,1,C_BLACK);
/*
	put_chinese_char16(220,5,0,C_BLACK);
	put_chinese_char16(250,5,1,C_BLACK);
	put_chinese_char16(280,5,2,C_BLACK);

	put_chinese_char32(80,70,1,C_BLACK);
	put_chinese_char32(120,70,2,C_BLACK);

	put_chinese_char32(80,150,3,C_BLACK);
	put_chinese_char32(120,150,4,C_BLACK);

	_HW_DrawLine(2,210,318,210,C_BLACK );
*/
	UG_WindowShow( &fastmem_select_exe );	

}

void fastmem_select_mem_window(void)
{
	UG_Update();	//Windows??

	UG_WindowCreate( &fastmem_select_mem, obj_buff_fsm_1, MAX_OBJECTS, fastmem_select_mem_callback );
	UG_WindowSetTitleTextFont( &fastmem_select_mem, &FONT_12X20 );
	UG_WindowSetTitleColor( &fastmem_select_mem, C_WHITE );


	UG_ButtonCreate( &fastmem_select_mem, &button_fsm_1, BTN_ID_0, 40, 40, 200, 80 );
	UG_ButtonSetFont( &fastmem_select_mem, BTN_ID_0, &FONT_22X36 );

	UG_ButtonCreate( &fastmem_select_mem, &button_fsm_2, BTN_ID_1, 40, 120, 200, 160 );
	UG_ButtonSetFont( &fastmem_select_mem, BTN_ID_1, &FONT_22X36 );
	
	//_UG_UpdateObjects( &fastmem_select_mem );

	put_chinese_char16(220,5,0,C_RED);
	put_chinese_char16(250,5,1,C_RED);
	put_chinese_char16(280,5,2,C_RED);

	put_chinese_char32(80,70,3,C_RED);
	put_chinese_char32(120,70,4,C_RED);

	put_chinese_char32(80,150,1,C_RED);
	put_chinese_char32(120,150,2,C_RED);

	//_HW_DrawLine(2,210,318,210,C_RED );
	
	//_UG_UpdateObjects( &fastmem_select_mem );
	UG_WindowShow( &fastmem_select_mem );	

}
void mainwindow()
{


}
void enterwindow()
{

	
}
void escwindow()
{

	
}
void upwindow()
{

	
}
void dowwindow()
{

	
}
void setwindow()
{

	
}


void UGUI_WindowStateHandler(eKeyType event)
{
	OS_ERR err;
	static int Key_Fun = 0;

	switch(event){	
		case Key_Func1: 
			UGUI_ShowChineseWindow();
			Key_Fun = MainKeyTab[Key_Fun].KeyEnterState;
		break;
		case Key_Func2: 		
			Key_Fun = MainKeyTab[Key_Fun].KeyBackState;
		break;
		case Key_PageUp: 	 
			Key_Fun = MainKeyTab[Key_Fun].KeyLeftState;
		break;
		case Key_Memo:		 
			Key_Fun = MainKeyTab[Key_Fun].KeyRightState;
		break;
		case 0:
		return;
		break;
	}
	KeyFuncPtr = MainKeyTab[Key_Fun].CurrentOperate ;
	(*KeyFuncPtr)();
	
}

/*
void UGUI_WindowStateHandler(eKeyType event)
{
	OS_ERR err;
	switch(event)
	{
<<<<<<< .mine
		case Key_Func1: 
			SPI_Flash_Read(logo_bmp,0xC0000000,21632);	
			UGUI_ShowSpash();		
			OSTimeDly(1000,OS_OPT_TIME_DLY,&err);
			SPI_Flash_Read(logo_bmp,0xC0010000,21632);	
			UGUI_ShowSpasha();				
		
||||||| .r14
		case Key_Func1:
			UGUI_ShowSpash();
=======
		case Key_Func1:
			//UGUI_ShowSpash();
			UGUI_WindowDemo();
>>>>>>> .r16
			printf("get key\r\n");
			break;
		case Key_Func2:					
			UGUI_ShowMainWindow();
			printf( "\r\n Key func2\r\n");
			break;
		case Key_PageUp: 
			SPI_Flash_Read(logo_bmp,0xC0010000,21632);	
			UGUI_ShowSubWindow();
			break;
//		case Key_SpeedAdj:
		case Key_LegCntSwitch:	//切换按钮

			if(led1on && led2on)
			{
				
				GPIO_SetBits(GPIOE,GPIO_Pin_2);
				GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			}else if(led1on && (!led2on))
			{
				
				GPIO_ResetBits(GPIOE,GPIO_Pin_2);
				GPIO_SetBits(GPIOE,GPIO_Pin_3);
			}else if((!led1on) && led2on)
			{
				
				GPIO_SetBits(GPIOE,GPIO_Pin_2);
				GPIO_SetBits(GPIOE,GPIO_Pin_3);
			}else
			{
				
				GPIO_SetBits(GPIOE,GPIO_Pin_2);
				GPIO_ResetBits(GPIOE,GPIO_Pin_3);
			}		
			break;
		case Key_Memo://记忆按钮
			fastmem_select_exe_window();
			break;
		case Key_BackUp: //背部上折
		    fastmem_select_mem_window();
			break;


		
		case Key_BackDown,//背部下折
		case Key_WaistUp,//腰部上折
		case Key_WaistDown,//腰部下折
		case Key_LegUp,//腿板上升
		case Key_LegDown,
		case Key_Trendelenburg,//头低脚高 前倾
		case Key_AntiTrendelenburg,//后倾
		case key_Up,//升高
		case Key_Down,//降低
		case Key_LeftLean,//左倾
		case Key_RightLean,//右倾
		case Key_HeadForward,//头部平移
		case Key_FootForward,//脚部平移
		case Key_ReverseFunc,//正反向按钮
		case Key_UnLock,//解锁
		case Key_Lock,//锁定
		case Key_Start,//启动
		case Key_Stop,//停止
		case Key_Reset,//零位
		case key_ReadData,
		case Key_Max	
		
	default:
			break;
	}
	
}

*/
