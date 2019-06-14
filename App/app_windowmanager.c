#include "app.h"
#include "image.h"
#include "delay.h"

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

/* Window 3 */
UG_WINDOW window_3;
UG_OBJECT obj_buff_wnd_3[MAX_OBJECTS];
UG_BUTTON button3_1;
UG_TEXTBOX textbox3_1;

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

/* -------------------------------------------------------------------------------- */
/* Create SplashWindow										                   */
/* -------------------------------------------------------------------------------- */

void UGUI_ShowSpash(void)
{
	UG_Update();	//Windows??


	UG_WindowCreate( &window_2, obj_buff_wnd_2, MAX_OBJECTS, window_2_callback );
	//UG_WindowSetTitleText( &window_2, "Info" );
	UG_WindowSetTitleTextFont( &window_2, &FONT_12X20 );
	UG_WindowSetTitleColor( &window_2, C_WHITE );
	//UG_WindowResize( &window_2, 20, 40, 219, 279 );
	
	put_chinese_char(250,150,0,C_BLACK);
	put_chinese_char(210,75,1,C_BLACK);
	
	put_chinese_char16(220,5,0,C_BLACK);
	put_chinese_char16(250,5,1,C_BLACK);
	put_chinese_char16(280,5,2,C_BLACK);

	
	/* Create Textbox 1 */
	UG_TextboxCreate( &window_2, &textbox2_1, TXB_ID_0, 10, 50, UG_WindowGetInnerWidth( &window_2 )-10, 100 );
	UG_TextboxSetFont( &window_2, TXB_ID_0, &FONT_22X36 );
	UG_TextboxSetText( &window_2, TXB_ID_0, "WEGO" );
	UG_TextboxSetAlignment( &window_2, TXB_ID_0, ALIGN_TOP_CENTER );
	UG_TextboxSetForeColor( &window_2, TXB_ID_0, C_BLACK );

	_HW_DrawLine(2,210,318,210,C_BLACK );

	UG_WindowShow( &window_2 );	




}

/* -------------------------------------------------------------------------------- */
/* Create  MainWindow										                   */
/* -------------------------------------------------------------------------------- */

void UGUI_ShowMainWindow(void)
{

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
	UG_Update();	//WindowsË¢ÐÂ

}

void UGUI_ShowSubWindow(void)
{
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

	UG_WindowShow( &window_2 ); 						
	UG_Update();	//WindowsË¢ÐÂ

}


void UGUI_WindowStateHandler(eKeyType event)
{
	switch(event)
	{
		case Key_Func1:
			UGUI_ShowSpash();
			printf("get key\r\n");
			break;
		case Key_Func2:	
			UGUI_ShowMainWindow();
			printf( "\r\n Key func2\r\n");
			break;
		case Key_PageUp: 
			UGUI_ShowSubWindow();
			break;
		default:
			break;
	}
	
}
