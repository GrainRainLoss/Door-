#include "nfc.h"
#include "delay.h"
#include "esp8266.h"

void COM1_2_Init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;	//≤®Ãÿ¬ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// ˝æ›Œª
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//Õ£÷πŒª
	USART_InitStructure.USART_Parity = USART_Parity_No;		//∆Ê≈º–£—È
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// ˝æ›¡˜øÿ÷∆
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ƒ£ Ω

	/* ø™∆ÙGPIO ±÷” */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 

	/* Configure USART Tx1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	/* Configure USART Rx1 as
	
	
	 floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


		/* Configure USART Tx2 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	/* Configure USART Rx2 as
	
	
	 floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable USART1 Receive interrupts */


  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		 
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
}
//≈‰÷√ ∏¡ø÷–∂œ£¨ ∏¡øµƒ“‚ÀºæÕ «”–À≥–Ú£¨”–œ»∫Ûµƒ“‚Àº°£
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//∂®“Â ˝æ›Ω·ππÃÂ
  NVIC_InitTypeDef NVIC_InitStructure2;	//∂®“Â ˝æ›Ω·ππÃÂ
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//Ω´÷–∂œ ∏¡ø∑≈µΩFlashµƒ0µÿ÷∑


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//…Ë÷√”≈œ»º∂≈‰÷√µƒƒ£ Ω£¨œÍ«È«Î‘ƒ∂¡‘≠≤ƒ¡œ÷–µƒŒƒ’¬

  // πƒ‹¥Æø⁄1÷–∂œ£¨≤¢…Ë÷√”≈œ»º∂
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//Ω´Ω·ππÃÂ∂™µΩ≈‰÷√∫Ø ˝£¨º¥–¥»ÎµΩ∂‘”¶ºƒ¥Ê∆˜÷–

    // πƒ‹¥Æø⁄2÷–∂œ£¨≤¢…Ë÷√”≈œ»º∂
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//Ω´Ω·ππÃÂ∂™µΩ≈‰÷√∫Ø ˝£¨º¥–¥»ÎµΩ∂‘”¶ºƒ¥Ê∆˜÷–

}
void Uart1_Send_Data(unsigned char *buf,unsigned char num)
{
	unsigned char i;
	for(i=0;i<num;i++)
	{ 
	 	USART_SendData(USART1, buf[i]);
	 	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}	
}

void Uart2_Send_Data(unsigned char *buf,unsigned char num)
{
	unsigned char i;
	for(i=0;i<num;i++)
	{ 
	 	USART_SendData(USART2, buf[i]);
	 	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}	
}
unsigned char RxCheckSum(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for(i=0;i<(len-1);i++)
	{
		   checksum ^= ptr[i];
	}
	checksum = ~checksum;
	if(ptr[len-1] == checksum)
		return 	STATUS_OK;
	else 
		return 	STATUS_ERR;
}

void TxCheckSum(unsigned char *ptr,unsigned char len)
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for(i=0;i<(len-1);i++)
	{
		   checksum ^= ptr[i];
	}
	checksum = ~checksum;
	ptr[len-1] = checksum;
}

//ReadId£®£©:∂¡ICø®ID∫≈£®ø®∫≈£©
//≤Œ ˝£∫*idout£¨∂¡»°µƒø®∫≈±£¥ÊµΩÀ¸À˘÷∏œÚµƒ¥Ê¥¢ø’º‰
//∑µªÿ÷µ£∫0£∫≥…π¶∂¡»°ø®∫≈£¨1£∫∂¡ø®∫≈ ß∞‹
unsigned char ReadId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Id[5] = 0x01;//ø™∆Ù∑‰√˘∆˜Ã· æ
	//Cmd_Read_Id[5] = 0x00;//πÿ±’∑‰√˘∆˜Ã· æ
	TxCheckSum(Cmd_Read_Id,Cmd_Read_Id[1]);		//º∆À„–£—È∫Õ
	esp8266_send_data("4",50);
//	Uart1_Send_Data(Cmd_Read_Id,Cmd_Read_Id[1]);		 //∑¢ÀÕ∂¡ø®∫≈ID√¸¡
	esp8266_send_data("5",50);
//	esp8266_send_data("pipixia",50);
//	esp8266_send_data(Cmd_Read_Id,Cmd_Read_Id[1]);
	delay_ms(150);//µ»¥˝ƒ£øÈ∑µªÿ ˝æ›£¨¥Û”⁄150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//∂‘Ω” ’µΩµƒ ˝æ›–£—È
		esp8266_send_data("6",50);
		if(status != STATUS_OK)  //≈–∂œ–£—È∫Õ «∑Ò’˝»∑
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK)	//≈–∂œ «∑Ò’˝»∑µƒ∂¡µΩø®
		{
		 	return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa1))//≈–∂œ «∑ÒŒ™∂¡ø®∫≈∑µªÿµƒ ˝æ›∞¸
		{
			for(i=0;i<6;i++)//ªÒ»°ø®∫≈ID£¨6◊÷Ω⁄		 
			{
				idout[i] = Uart1RxBuf[i+5];//¥” ˝◊Èµƒµ⁄5∏ˆ◊÷Ω⁄ø™ ºŒ™ø®∫≈£¨≥§∂»Œ™6◊÷Ω⁄
			}
			return STATUS_OK;		 //≥…π¶∑µªÿ0
		}
 	} 
	return STATUS_ERR;			// ß∞‹∑µªÿ1
}
//ReadId£®£©:∂¡ICø® ˝æ›øÈ
//≤Œ ˝£∫*idout£¨∂¡»°µƒ ˝æ›±£¥ÊµΩÀ¸À˘÷∏œÚµƒ¥Ê¥¢ø’º‰
//≤Œ ˝£∫block£¨øÈ∫≈
//∑µªÿ÷µ£∫0£∫≥…π¶∂¡»°£¨1£∫∂¡∂¡»° ß∞‹
unsigned char ReadDataFromBlock(unsigned char *dataout,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Block[4] = block;
	Cmd_Read_Block[5] = 0x01;//ø™∆Ù∑‰√˘∆˜Ã· æ
//	Cmd_Read_Block[5] = 0x00;//πÿ±’∑‰√˘∆˜Ã· æ
	TxCheckSum(Cmd_Read_Block,Cmd_Read_Block[1]);	// ˝æ›–£—È
	esp8266_send_data("pipixia",50);
	Uart1_Send_Data(Cmd_Read_Block,Cmd_Read_Block[1]);		 //∑¢ÀÕ∂¡ ˝æ›øÈ√¸¡Ó
	delay_ms(150);//µ»¥˝ƒ£øÈ∑µªÿ ˝æ›£¨¥Û”⁄150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//∂‘Ω” ’µΩµƒ ˝æ›–£—È
		if(status != STATUS_OK)		 //≈–∂œ–£—È∫Õ «∑Ò’˝»∑
		{
			return 	STATUS_ERR;
		}
		status = Uart1RxBuf[4];		//ªÒ»°∑µªÿ∞¸◊¥Ã¨
		if(status != STATUS_OK)	//≈–∂œ «∑Ò’˝»∑µƒ∂¡µΩø®
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa3))//≈–∂œ «∑ÒŒ™∂¡øÈ ˝æ›∑µªÿµƒ ˝æ›∞¸
		{
			for(i=0;i<16;i++)//ªÒ»°øÈ ˝æ›£¨16◊÷Ω⁄	£¨“ª∏ˆ ˝æ›øÈµƒ¥Û–°Œ™16◊÷Ω⁄	 
			{
				dataout[i] = Uart1RxBuf[i+5];//¥” ˝◊Èµƒµ⁄5∏ˆ◊÷Ω⁄ø™ ºŒ™ ˝æ›£¨≥§∂»Œ™16◊÷Ω⁄
			}
			return STATUS_OK;		 //≥…π¶∑µªÿ0
		}
	}
	return STATUS_ERR;			// ß∞‹∑µªÿ1
}
//ReadId£®£©:–¥ ˝æ›µΩ÷∏∂®µƒ ˝æ›øÈ
//≤Œ ˝£∫*idout£¨÷∏œÚ“™–¥»Î ˝æ›µƒª∫≥Â«¯
//≤Œ ˝£∫block£¨øÈ∫≈
//∑µªÿ÷µ£∫0£∫–¥»Î≥…π¶£¨1£∫–¥»Î ß∞‹
unsigned char WriteDataToBlock(unsigned char *datain,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Write_Block[4] = block;
	for(i=0;i<16;i++)
	{
		Cmd_Write_Block[6+i] = datain[i];
	}
	TxCheckSum(Cmd_Write_Block,Cmd_Write_Block[1]);	// ˝æ›–£—È
	Uart1_Send_Data(Cmd_Write_Block,Cmd_Write_Block[1]);		 //∑¢ÀÕ–¥√¸¡Ó
	delay_ms(150);//µ»¥˝ƒ£øÈ∑µªÿ ˝æ›£¨¥Û”⁄150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//∂‘∑µªÿµƒ ˝æ›Ω¯–––£—È
		if(status != STATUS_OK) //≈–∂œ–£—È «∑ÒÕ®π˝
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK) //≈–∂œ–£—È «∑ÒÕ®π˝
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa4))//≈–∂œ «∑ÒŒ™–¥øÈ ˝æ›∑µªÿµƒ ˝æ›∞¸
		{
				return STATUS_OK;		 //≥…π¶∑µªÿ0
		}
 	} 
	return STATUS_ERR;			// ß∞‹∑µªÿ1
}