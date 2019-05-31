#include "ugui.h"

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
