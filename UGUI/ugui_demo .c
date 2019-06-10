#ifdef USE_DEMO

#include "ugui_demo.h"
#include "image.h"
#include "delay.h"
int i;


void GUI_Demo(void)
{
		/* Init ?UI */
	 UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))_HW_DrawPoint,320,240);

		/* Register hardware acceleration */
	 UG_DriverRegister( DRIVER_DRAW_LINE, (void*)_HW_DrawLine );
	 UG_DriverRegister( DRIVER_FILL_FRAME, (void*)_HW_FillFrame );
	 UG_DriverEnable( DRIVER_DRAW_LINE );
	 UG_DriverEnable( DRIVER_FILL_FRAME );
	 UG_FillScreen( C_BLACK );
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
   /* Create Window 2 (?UI Info)                                                      */
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
   /* Create Window 3 (Benchmark Result)                                               */
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
   /* Start demo application                                                           */
   /* -------------------------------------------------------------------------------- */
   /* Show Window 1 */
	 delay_ms(1000);
  while(1)	
	{
	      UG_WindowShow( &window_1 );				
				delay_ms(2000);				
				UG_Update();	//WindowsË¢ÐÂ
				UG_WindowShow( &window_2 );
				delay_ms(3000);
				UG_Update();	//WindowsË¢ÐÂ
				//UG_WindowShow( &window_3 );
				//delay_ms(1000);
				//UG_Update();	//WindowsË¢ÐÂ
	}

}

#endif
