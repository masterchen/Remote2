#include "bsp.h"
#include "crc16.h"
#include "app.h"
#define COM_TIMEOUT   80 //50ms


u8 appCOM_SendCmdCode(u8 _CmdCode)
{
	/* 6 字节指令，2字节校验*/
	u8 tmpData[6+2] = {0x00,0x06,0x00,0x46,0x00,0x00,0xff,0xff};
	u8 size = 0;
	u8 charTemp[8] = {0};
	u8 i=0;
	s16 timeout = COM_TIMEOUT;
	OS_ERR err;
	//memset(tmpData, 0, sizeof(tmpData));
	
	for(i=0; i<3; i++){
		tmpData[5] = _CmdCode;
		tmpData[6] = crc16(tmpData,6) & 0xff;
		tmpData[7] = crc16(tmpData,6) >> 8 & 0xff;
		comSendBuf(COM2, tmpData, 8);
		timeout = COM_TIMEOUT;
		while(timeout >0){
			if(comGetChar(COM2, &charTemp[size]) == 1){
				size++;
				printf("Got reply: 0x%x",charTemp[size]);
				if(size == 8)
					break;
				if(charTemp[6] != tmpData[6] || charTemp[7] != tmpData[7])
				{
					printf("checksum error!\r\n");
				}
			}
			OSTimeDlyHMSM(0, 0,0,10,OS_OPT_TIME_HMSM_STRICT,&err);
			timeout -= 10;
		}
	}

	if(i>=3){
		printf("timeout!\r\n");
	}
	return TRUE;
}


