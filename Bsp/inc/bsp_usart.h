/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
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

/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 ��  PB6, PB7*/
	COM2 = 1,	/* USART2, PD5,PD6 �� PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
	COM6 = 5,	/* USART6, PC6, PC7 */
	COM_MAX,
}COM_PORT_E;

extern s32 s_BaudRate[COM_MAX];

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
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
 

/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	volatile uint8_t *pTxBuf;			/* ���ͻ����� */
	volatile uint8_t *pRxBuf;			/* ���ջ����� */
	volatile uint16_t usTxBufSize;		/* ���ͻ�������С */
	volatile uint16_t usRxBufSize;		/* ���ջ�������С */
	volatile uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	volatile uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	volatile uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	volatile uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	volatile uint16_t usRxRead;			/* ���ջ�������ָ�� */
	volatile uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

}UART_T;

void bsp_InitUart(COM_PORT_E _ucPort, s32 BaudRate);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

#endif

