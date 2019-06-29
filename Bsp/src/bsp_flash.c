#include "bsp_flash.h" 
#include "bsp_spi.h"
#include "delay.h"   

//4KbytesÎªÒ»¸öSector
//16¸öÉÈÇøÎª1¸öBlock
//W25X16
//ÈİÁ¿Îª2M×Ö½Ú,¹²ÓĞ32¸öBlock,512¸öSector 

//³õÊ¼»¯SPI FLASHµÄIO¿Ú

/*
W25Q128 ½« 16M µÄÈİÁ¿·ÖÎª 256 ¸ö¿é£¨Block£©£¬Ã¿¸ö¿é´óĞ¡Îª 64K ×Ö½Ú£¬Ã¿¸ö¿éÓÖ·ÖÎª 16¸öÉÈÇø£¨Sector£©£
Ã¿¸öÉÈÇø 4K ¸ö×Ö½Ú¡£ W25Q128 µÄ×îÉÙ²Á³ıµ¥Î»ÎªÒ»¸öÉÈÇø£¬Ò²¾ÍÊÇÃ¿´Î±ØĞë²Á³ı 4K ¸ö×Ö½Ú¡£
²Ù×÷ĞèÒª¸ø W25Q128 ¿ª±ÙÒ»¸öÖÁÉÙ 4K µÄ»º´æÇø£¬¶Ô SRAM ÒªÇó±È½Ï¸ß£¬ÒªÇóĞ¾Æ¬±ØĞëÓĞ 4K ÒÔÉÏ SRAM ²ÅÄÜºÜºÃµÄ²Ù×÷¡£
*/

void SPI_Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE ); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);

	SPIx_Init();		   //³õÊ¼»¯SPI
    SPIx_SetSpeed(SPI_BaudRatePrescaler_2);//ÉèÖÃÎª18MÊ±ÖÓ,¸ßËÙÄ£Ê½	
}  

//¶ÁÈ¡SPI_FLASHµÄ×´Ì¬¼Ä´æÆ÷
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ä¬ÈÏ0,×´Ì¬¼Ä´æÆ÷±£»¤Î»,ÅäºÏWPÊ¹ÓÃ
//TB,BP2,BP1,BP0:FLASHÇøÓòĞ´±£»¤ÉèÖÃ
//WEL:Ğ´Ê¹ÄÜËø¶¨
//BUSY:Ã¦±ê¼ÇÎ»(1,Ã¦;0,¿ÕÏĞ)
//Ä¬ÈÏ:0x00
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
	SPIx_ReadWriteByte(W25X_ReadStatusReg);    //·¢ËÍ¶ÁÈ¡×´Ì¬¼Ä´æÆ÷ÃüÁî    
	byte=SPIx_ReadWriteByte(0Xff);             //¶ÁÈ¡Ò»¸ö×Ö½Ú  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     
	return byte;   
} 
//Ğ´SPI_FLASH×´Ì¬¼Ä´æÆ÷
//Ö»ÓĞSPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)¿ÉÒÔĞ´!!!
void SPI_FLASH_Write_SR(u8 sr)   
{   
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
	SPIx_ReadWriteByte(W25X_WriteStatusReg);   //·¢ËÍĞ´È¡×´Ì¬¼Ä´æÆ÷ÃüÁî    
	SPIx_ReadWriteByte(sr);               //Ğ´ÈëÒ»¸ö×Ö½Ú  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
}   
//SPI_FLASHĞ´Ê¹ÄÜ	
//½«WELÖÃÎ»   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_WriteEnable);      //·¢ËÍĞ´Ê¹ÄÜ  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
} 
//SPI_FLASHĞ´½ûÖ¹	
//½«WELÇåÁã  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_WriteDisable);     //·¢ËÍĞ´½ûÖ¹Ö¸Áî    
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
} 			    
//¶ÁÈ¡Ğ¾Æ¬ID W25X16µÄID:0XEF14
u16 SPI_Flash_ReadID(void)
{
	u16 Temp = 0;	  
	SPI_FLASH_CS=0;				    
	SPIx_ReadWriteByte(0x90);//·¢ËÍ¶ÁÈ¡IDÃüÁî	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	    
	SPIx_ReadWriteByte(0x00); 	 			   
	Temp|=SPIx_ReadWriteByte(0xFF)<<8;  
	Temp|=SPIx_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS=1;				    
	return Temp;
}   		    
//¶ÁÈ¡SPI FLASH  
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼¶ÁÈ¡Ö¸¶¨³¤¶ÈµÄÊı¾İ
//pBuffer:Êı¾İ´æ´¢Çø
//ReadAddr:¿ªÊ¼¶ÁÈ¡µÄµØÖ·(24bit)
//NumByteToRead:Òª¶ÁÈ¡µÄ×Ö½ÚÊı(×î´ó65535)
void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_ReadData);         //·¢ËÍ¶ÁÈ¡ÃüÁî   
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));  //·¢ËÍ24bitµØÖ·    
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPIx_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);   //Ñ­»·¶ÁÊı  
    }
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
}  
//SPIÔÚÒ»Ò³(0~65535)ÄÚĞ´ÈëÉÙÓÚ256¸ö×Ö½ÚµÄÊı¾İ
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´Èë×î´ó256×Ö½ÚµÄÊı¾İ
//pBuffer:Êı¾İ´æ´¢Çø
//WriteAddr:¿ªÊ¼Ğ´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªĞ´ÈëµÄ×Ö½ÚÊı(×î´ó256),¸ÃÊı²»Ó¦¸Ã³¬¹ı¸ÃÒ³µÄÊ£Óà×Ö½ÚÊı!!!	 
void SPI_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    SPI_FLASH_Write_Enable();                  //SET WEL 
	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_PageProgram);      //·¢ËÍĞ´Ò³ÃüÁî   
    SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); //·¢ËÍ24bitµØÖ·    
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPIx_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//Ñ­»·Ğ´Êı  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡ 
	SPI_Flash_Wait_Busy();					   //µÈ´ıĞ´Èë½áÊø
} 
//ÎŞ¼ìÑéĞ´SPI FLASH 
//±ØĞëÈ·±£ËùĞ´µÄµØÖ··¶Î§ÄÚµÄÊı¾İÈ«²¿Îª0XFF,·ñÔòÔÚ·Ç0XFF´¦Ğ´ÈëµÄÊı¾İ½«Ê§°Ü!
//¾ßÓĞ×Ô¶¯»»Ò³¹¦ÄÜ 
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨³¤¶ÈµÄÊı¾İ,µ«ÊÇÒªÈ·±£µØÖ·²»Ô½½ç!
//pBuffer:Êı¾İ´æ´¢Çø
//WriteAddr:¿ªÊ¼Ğ´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªĞ´ÈëµÄ×Ö½ÚÊı(×î´ó65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //µ¥Ò³Ê£ÓàµÄ×Ö½ÚÊı		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//²»´óÓÚ256¸ö×Ö½Ú
	while(1)
	{	   
		SPI_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//Ğ´Èë½áÊøÁË
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //¼õÈ¥ÒÑ¾­Ğ´ÈëÁËµÄ×Ö½ÚÊı
			if(NumByteToWrite>256)pageremain=256; //Ò»´Î¿ÉÒÔĞ´Èë256¸ö×Ö½Ú
			else pageremain=NumByteToWrite; 	  //²»¹»256¸ö×Ö½ÚÁË
		}
	};	    
} 
//Ğ´SPI FLASH  
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨³¤¶ÈµÄÊı¾İ
//¸Ãº¯Êı´ø²Á³ı²Ù×÷!
//pBuffer:Êı¾İ´æ´¢Çø
//WriteAddr:¿ªÊ¼Ğ´ÈëµÄµØÖ·(24bit)
//NumByteToWrite:ÒªĞ´ÈëµÄ×Ö½ÚÊı(×î´ó65535)  		   
u8 SPI_FLASH_BUF[4096];
void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    

	secpos=WriteAddr/4096;//ÉÈÇøµØÖ· 0~511 for w25x16
	secoff=WriteAddr%4096;//ÔÚÉÈÇøÄÚµÄÆ«ÒÆ
	secremain=4096-secoff;//ÉÈÇøÊ£Óà¿Õ¼ä´óĞ¡   

	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//²»´óÓÚ4096¸ö×Ö½Ú
	while(1) 
	{	
		SPI_Flash_Read(SPI_FLASH_BUF,secpos*4096,4096);//¶Á³öÕû¸öÉÈÇøµÄÄÚÈİ
		for(i=0;i<secremain;i++)//Ğ£ÑéÊı¾İ
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//ĞèÒª²Á³ı  	  
		}
		if(i<secremain)//ĞèÒª²Á³ı
		{
			SPI_Flash_Erase_Sector(secpos);//²Á³ıÕâ¸öÉÈÇø
			for(i=0;i<secremain;i++)	   //¸´ÖÆ
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF,secpos*4096,4096);//Ğ´ÈëÕû¸öÉÈÇø  

		}else SPI_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);//Ğ´ÒÑ¾­²Á³ıÁËµÄ,Ö±½ÓĞ´ÈëÉÈÇøÊ£ÓàÇø¼ä. 				   
		if(NumByteToWrite==secremain)break;//Ğ´Èë½áÊøÁË
		else//Ğ´ÈëÎ´½áÊø
		{
			secpos++;//ÉÈÇøµØÖ·Ôö1
			secoff=0;//Æ«ÒÆÎ»ÖÃÎª0 	 

		   	pBuffer+=secremain;  //Ö¸ÕëÆ«ÒÆ
			WriteAddr+=secremain;//Ğ´µØÖ·Æ«ÒÆ	   
		   	NumByteToWrite-=secremain;				//×Ö½ÚÊıµİ¼õ
			if(NumByteToWrite>4096)secremain=4096;	//ÏÂÒ»¸öÉÈÇø»¹ÊÇĞ´²»Íê
			else secremain=NumByteToWrite;			//ÏÂÒ»¸öÉÈÇø¿ÉÒÔĞ´ÍêÁË
		}	 
	};	 	 
}
//²Á³ıÕû¸öĞ¾Æ¬
//ÕûÆ¬²Á³ıÊ±¼ä:
//W25X16:25s 
//W25X32:40s 
//W25X64:40s 
//µÈ´ıÊ±¼ä³¬³¤...
void SPI_Flash_Erase_Chip(void)   
{                                             
    SPI_FLASH_Write_Enable();                  //SET WEL 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_ChipErase);        //·¢ËÍÆ¬²Á³ıÃüÁî  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
	SPI_Flash_Wait_Busy();   				   //µÈ´ıĞ¾Æ¬²Á³ı½áÊø
}   
//²Á³ıÒ»¸öÉÈÇø
//Dst_Addr:ÉÈÇøµØÖ· 0~511 for w25x16
//²Á³ıÒ»¸öÉ½ÇøµÄ×îÉÙÊ±¼ä:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
	Dst_Addr*=4096;
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   
  	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_SectorErase);      //·¢ËÍÉÈÇø²Á³ıÖ¸Áî 
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));  //·¢ËÍ24bitµØÖ·    
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPIx_ReadWriteByte((u8)Dst_Addr);  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
    SPI_Flash_Wait_Busy();   				   //µÈ´ı²Á³ıÍê³É
}  
//µÈ´ı¿ÕÏĞ
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   // µÈ´ıBUSYÎ»Çå¿Õ
}  
//½øÈëµôµçÄ£Ê½
void SPI_Flash_PowerDown(void)   
{ 
  	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_PowerDown);        //·¢ËÍµôµçÃüÁî  
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
    delay_us(3);                               //µÈ´ıTPD  
}   
//»½ĞÑ
void SPI_Flash_WAKEUP(void)   
{  
  	SPI_FLASH_CS=0;                            //Ê¹ÄÜÆ÷¼ş   
    SPIx_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	SPI_FLASH_CS=1;                            //È¡ÏûÆ¬Ñ¡     	      
    delay_us(3);                               //µÈ´ıTRES1
}   


























