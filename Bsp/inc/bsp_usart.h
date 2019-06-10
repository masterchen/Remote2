/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	
*
*********************************************************************************************************
*/

#ifndef _USART_H_
#define _USART_H_
#include "stdio.h"	
#include "sys.h" 
#include "stm32f10x.h"


#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	1

 

typedef enum
{
    CB_TYPE_SENDBEFORE,
    CB_TYPE_SENDOVER,
    CB_TYPE_NEWCOME,
}eCB_TYPE;

/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 或  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 或 PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5,	/* USART6, PC6, PC7 */
	COM_MAX,
}COM_PORT_E;

extern s32 s_BaudRate[COM_MAX];

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			s_BaudRate[COM1]//115200
	#define UART1_TX_BUF_SIZE	1*64
	#define UART1_RX_BUF_SIZE	1*64
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			s_BaudRate[COM2]//38400//9600
	#define UART2_TX_BUF_SIZE	1*64
	#define UART2_RX_BUF_SIZE	1*64
#endif
 

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	volatile uint8_t *pTxBuf;			/* 发送缓冲区 */
	volatile uint8_t *pRxBuf;			/* 接收缓冲区 */
	volatile uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	volatile uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	volatile uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	volatile uint16_t usTxRead;			/* 发送缓冲区读指针 */
	volatile uint16_t usTxCount;			/* 等待发送的数据个数 */

	volatile uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	volatile uint16_t usRxRead;			/* 接收缓冲区读指针 */
	volatile uint16_t usRxCount;			/* 还未读取的新数据个数 */

}UART_T;

void bsp_InitUart(COM_PORT_E _ucPort, s32 BaudRate);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

#endif

