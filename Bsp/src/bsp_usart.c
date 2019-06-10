/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.c
*	��    �� : V1.0
*	˵    �� : ���ô����ж�+FIFOģʽʵ�ֶ�����ڵ�ͬʱ����
*	
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "app_cfg.h"
#include "stm32f10x.h"



#if 1
//#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 



s32 s_BaudRate[COM_MAX] = {115200, 38400, 38400, 9600, 9600, 9600};
//u8 g_bLogLevel = TRACE_INFO;

/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	static UART_T g_tUart1;
	static volatile uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	static volatile uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
    void USART1_IRQHandler(void);
#endif

#if UART2_FIFO_EN == 1
	static UART_T g_tUart2;
	static volatile uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	static volatile uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
    void USART2_IRQHandler(void);
#endif

 
static u8 s_bInited[COM_MAX] = {0};

static void UartVarInit(COM_PORT_E _ucPort);

static void InitHardUart(COM_PORT_E _ucPort);
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen);
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte);
static void UartIRQ(UART_T *_pUart);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ������Ӳ��������ȫ�ֱ�������ֵ.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(COM_PORT_E _ucPort, s32 BaudRate)
{
    s_BaudRate[_ucPort%COM_MAX] = BaudRate;
    UartVarInit(_ucPort);		/* �����ȳ�ʼ��ȫ�ֱ���,������Ӳ�� */

    InitHardUart(_ucPort);		/* ���ô��ڵ�Ӳ������(�����ʵ�) */ 
    s_bInited[_ucPort%COM_MAX] = TRUE;
}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: uartָ��
*********************************************************************************************************
*/
UART_T *ComToUart(COM_PORT_E _ucPort)
{
	if (_ucPort == COM1)
	{
		#if UART1_FIFO_EN == 1
			return &g_tUart1;
		#else
			return 0;
		#endif
	}
	else if (_ucPort == COM2)
	{
		#if UART2_FIFO_EN == 1
			return &g_tUart2;
		#else
			return 0;
		#endif
	} 
	else
	{
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: comSendBuf
*	����˵��: �򴮿ڷ���һ�����ݡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucaBuf: �����͵����ݻ�����
*			  _usLen : ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen)
{
	UART_T *pUart;

    if(s_bInited[_ucPort] == FALSE){
        return;
    }

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	UartSend(pUart, _ucaBuf, _usLen);
}

/*
*********************************************************************************************************
*	�� �� ��: comSendChar
*	����˵��: �򴮿ڷ���1���ֽڡ����ݷŵ����ͻ��������������أ����жϷ�������ں�̨��ɷ���
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _ucByte: �����͵�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte)
{
	comSendBuf(_ucPort, &_ucByte, 1);
}

/*
*********************************************************************************************************
*	�� �� ��: comGetChar
*	����˵��: �Ӵ��ڻ�������ȡ1�ֽڣ��������������������ݾ���������
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*			  _pByte: ���յ������ݴ���������ַ
*	�� �� ֵ: 0 ��ʾ������, 1 ��ʾ��ȡ����Ч�ֽ�
*********************************************************************************************************
*/
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte)
{
	UART_T *pUart;

    if(s_bInited[_ucPort] == FALSE){
        return 0;
    }

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return 0;
	}

	return UartGetChar(pUart, _pByte);
}

/*
*********************************************************************************************************
*	�� �� ��: comClearTxFifo
*	����˵��: ���㴮�ڷ��ͻ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearTxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usTxWrite = 0;
	pUart->usTxRead = 0;
	pUart->usTxCount = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: comClearRxFifo
*	����˵��: ���㴮�ڽ��ջ�����
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void comClearRxFifo(COM_PORT_E _ucPort)
{
	UART_T *pUart;

	pUart = ComToUart(_ucPort);
	if (pUart == 0)
	{
		return;
	}

	pUart->usRxWrite = 0;
	pUart->usRxRead = 0;
	pUart->usRxCount = 0;
}

 
/*
*********************************************************************************************************
*	�� �� ��: UartVarInit
*	����˵��: ��ʼ��������صı���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartVarInit(COM_PORT_E _ucPort)
{
    switch(_ucPort){
    case COM1:
#if UART1_FIFO_EN == 1
    	g_tUart1.uart = USART1;						/* STM32 �����豸 */
    	g_tUart1.pTxBuf = g_TxBuf1;					/* ���ͻ�����ָ�� */
    	g_tUart1.pRxBuf = g_RxBuf1;					/* ���ջ�����ָ�� */
    	g_tUart1.usTxBufSize = UART1_TX_BUF_SIZE;	/* ���ͻ�������С */
    	g_tUart1.usRxBufSize = UART1_RX_BUF_SIZE;	/* ���ջ�������С */
    	g_tUart1.usTxWrite = 0;						/* ����FIFOд���� */
    	g_tUart1.usTxRead = 0;						/* ����FIFO������ */
    	g_tUart1.usRxWrite = 0;						/* ����FIFOд���� */
    	g_tUart1.usRxRead = 0;						/* ����FIFO������ */
    	g_tUart1.usRxCount = 0;						/* ���յ��������ݸ��� */
    	g_tUart1.usTxCount = 0;						/* �����͵����ݸ��� */
#endif
    break;

    case COM2:
#if UART2_FIFO_EN == 1
    	g_tUart2.uart = USART2;						/* STM32 �����豸 */
    	g_tUart2.pTxBuf = g_TxBuf2;					/* ���ͻ�����ָ�� */
    	g_tUart2.pRxBuf = g_RxBuf2;					/* ���ջ�����ָ�� */
    	g_tUart2.usTxBufSize = UART2_TX_BUF_SIZE;	/* ���ͻ�������С */
    	g_tUart2.usRxBufSize = UART2_RX_BUF_SIZE;	/* ���ջ�������С */
    	g_tUart2.usTxWrite = 0;						/* ����FIFOд���� */
    	g_tUart2.usTxRead = 0;						/* ����FIFO������ */
    	g_tUart2.usRxWrite = 0;						/* ����FIFOд���� */
    	g_tUart2.usRxRead = 0;						/* ����FIFO������ */
    	g_tUart2.usRxCount = 0;						/* ���յ��������ݸ��� */
    	g_tUart2.usTxCount = 0;						/* �����͵����ݸ��� */	
#endif
    break; 
    default: 
        break;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: InitHardUart
*	����˵��: ���ô��ڵ�Ӳ�������������ʣ�����λ��ֹͣλ����ʼλ��У��λ���ж�ʹ�ܣ��ʺ���STM32-F4������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitHardUart(COM_PORT_E _ucPort)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

    switch(_ucPort)
		{ 
        case COM1:
			//GPIO�˿�����	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��
		USART_DeInit(USART1);  //��λ����1
		//USART1_TX   PA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

		//USART1_RX   PA.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��PA10

		//Usart1 NVIC ����

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

		 //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = UART1_BAUD;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

		USART_Init(USART1, &USART_InitStructure); //��ʼ������
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
		USART_Cmd(USART1, ENABLE);					  //ʹ�ܴ��� 

    break;

    case COM2:
 		RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE); //ʹ��USART1��GPIOAʱ��
		USART_DeInit(USART2);  //��λ����1
		//USART1_TX   PA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

		//USART1_RX   PA.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��PA10

		//Usart1 NVIC ����

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

		 //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = UART2_BAUD;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

		USART_Init(USART2, &USART_InitStructure); //��ʼ������
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
		USART_Cmd(USART2, ENABLE);		
    break; 
		default:
			break;
	}
}
 
/*
*********************************************************************************************************
*	�� �� ��: UartSend
*	����˵��: ��д���ݵ�UART���ͻ�����,�����������жϡ��жϴ�����������Ϻ��Զ��رշ����ж�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartSend(UART_T *_pUart, uint8_t *_ucaBuf, uint16_t _usLen)
{
	uint16_t i;

	for (i = 0; i < _usLen; i++)
	{
		/* ������ͻ������Ѿ����ˣ���ȴ��������� */		
	#if 0
		/* 
			�ڵ���GPRS����ʱ������Ĵ������������while ��ѭ��
			ԭ�� ���͵�1���ֽ�ʱ _pUart->usTxWrite = 1��_pUart->usTxRead = 0;
			������while(1) �޷��˳�
		*/
		while (1)
		{
			uint16_t usRead;

			DISABLE_INT();
			usRead = _pUart->usTxRead;
			ENABLE_INT();

			if (++usRead >= _pUart->usTxBufSize)
			{
				usRead = 0;
			}

			if (usRead != _pUart->usTxWrite)
			{
				break;
			}
		}
	#else
    /*
		while (1)
		{
			uint16_t usCount;

			DISABLE_INT();
			usCount = _pUart->usTxCount;
			ENABLE_INT();

			if (usCount < _pUart->usTxBufSize)
			{
				break;
			}
		}	
		*/
	#endif

		/* �����������뷢�ͻ����� */
		_pUart->pTxBuf[_pUart->usTxWrite] = _ucaBuf[i];

		//DISABLE_INT();
		USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);
        
		if (++_pUart->usTxWrite >= _pUart->usTxBufSize)
		{
			_pUart->usTxWrite = 0;
		}
		_pUart->usTxCount++;
        if (_pUart->usTxCount >= _pUart->usTxBufSize)
        {
            _pUart->usTxCount = _pUart->usTxBufSize;
        }
		//ENABLE_INT();
        USART_ITConfig(_pUart->uart, USART_IT_TXE, ENABLE);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: UartGetChar
*	����˵��: �Ӵ��ڽ��ջ�������ȡ1�ֽ����� ��������������ã�
*	��    ��: _pUart : �����豸
*			  _pByte : ��Ŷ�ȡ���ݵ�ָ��
*	�� �� ֵ: 0 ��ʾ������  1��ʾ��ȡ������
*********************************************************************************************************
*/
static uint8_t UartGetChar(UART_T *_pUart, uint8_t *_pByte)
{
	uint16_t usCount;

	/* usRxWrite �������жϺ����б���д���������ȡ�ñ���ʱ����������ٽ������� */
	//DISABLE_INT();
	USART_ITConfig(_pUart->uart, USART_IT_RXNE, DISABLE);
	usCount = _pUart->usRxCount;
	USART_ITConfig(_pUart->uart, USART_IT_RXNE, ENABLE);
	//ENABLE_INT();

	/* �������д������ͬ���򷵻�0 */
	//if (_pUart->usRxRead == usRxWrite)
	if (usCount == 0)	/* �Ѿ�û������ */
	{
		return 0;
	}
	else
	{
		*_pByte = _pUart->pRxBuf[_pUart->usRxRead];		/* �Ӵ��ڽ���FIFOȡ1������ */

		/* ��дFIFO������ */
		//DISABLE_INT();
        USART_ITConfig(_pUart->uart, USART_IT_RXNE, DISABLE);
		if (++_pUart->usRxRead >= _pUart->usRxBufSize)
		{
			_pUart->usRxRead = 0;
		}
		_pUart->usRxCount--;
        USART_ITConfig(_pUart->uart, USART_IT_RXNE, ENABLE);
		//ENABLE_INT();
		return 1;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: UartIRQ
*	����˵��: ���жϷ��������ã�ͨ�ô����жϴ�����
*	��    ��: _pUart : �����豸
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void UartIRQ(UART_T *_pUart)
{
	/* ��������ж�  */
	if (USART_GetITStatus(_pUart->uart, USART_IT_RXNE) != RESET)
	{
	    u8 uData = 0;
		/* �Ӵ��ڽ������ݼĴ�����ȡ���ݴ�ŵ�����FIFO */
        uData = USART_ReceiveData(_pUart->uart);
		_pUart->pRxBuf[_pUart->usRxWrite] = uData;//USART_ReceiveData(_pUart->uart);
		if (++_pUart->usRxWrite >= _pUart->usRxBufSize)
		{
			_pUart->usRxWrite = 0;
		}
		if (_pUart->usRxCount < _pUart->usRxBufSize)
		{
			_pUart->usRxCount++;
		}
	
	}

	/* �����ͻ��������ж� */
	if (USART_GetITStatus(_pUart->uart, USART_IT_TXE) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* ���ͻ�������������ȡ��ʱ�� ��ֹ���ͻ��������ж� ��ע�⣺��ʱ���1�����ݻ�δ����������ϣ�*/
			USART_ITConfig(_pUart->uart, USART_IT_TXE, DISABLE);

			/* ʹ�����ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, ENABLE);
		}
		else
		{
			/* �ӷ���FIFOȡ1���ֽ�д�봮�ڷ������ݼĴ��� */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}

	}
	/* ����bitλȫ��������ϵ��ж� */
	else if (USART_GetITStatus(_pUart->uart, USART_IT_TC) != RESET)
	{
		//if (_pUart->usTxRead == _pUart->usTxWrite)
		if (_pUart->usTxCount == 0)
		{
			/* �������FIFO������ȫ��������ϣ���ֹ���ݷ�������ж� */
			USART_ITConfig(_pUart->uart, USART_IT_TC, DISABLE);
		}
		else
		{
			/* ��������£��������˷�֧ */
			
			/* �������FIFO�����ݻ�δ��ϣ���ӷ���FIFOȡ1������д�뷢�����ݼĴ��� */
			USART_SendData(_pUart->uart, _pUart->pTxBuf[_pUart->usTxRead]);
			if (++_pUart->usTxRead >= _pUart->usTxBufSize)
			{
				_pUart->usTxRead = 0;
			}
			_pUart->usTxCount--;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler  USART2_IRQHandler USART3_IRQHandler UART4_IRQHandler UART5_IRQHandler
*	����˵��: USART�жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
#if UART1_FIFO_EN == 1
void USART1_IRQHandler(void)
{ 
	UartIRQ(&g_tUart1); 
}
#endif

#if UART2_FIFO_EN == 1
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tUart2);
}
#endif

  //����1�����ַ���
void Uart1SendStr(u8 *s)
{
  while (*s)                  //����ַ���������־
    {
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,*s++);

    }

}


