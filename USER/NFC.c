#include "nfc.h"
#include "delay.h"
#include "esp8266.h"

void COM1_2_Init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;	//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//模式

	/* 开启GPIO时钟 */
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
//配置矢量中断，矢量的意思就是有顺序，有先后的意思。
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//定义数据结构体
  NVIC_InitTypeDef NVIC_InitStructure2;	//定义数据结构体
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//将中断矢量放到Flash的0地址


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//设置优先级配置的模式，详情请阅读原材料中的文章

  //使能串口1中断，并设置优先级
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//将结构体丢到配置函数，即写入到对应寄存器中

    //使能串口2中断，并设置优先级
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//将结构体丢到配置函数，即写入到对应寄存器中

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

//ReadId（）:读IC卡ID号（卡号）
//参数：*idout，读取的卡号保存到它所指向的存储空间
//返回值：0：成功读取卡号，1：读卡号失败
unsigned char ReadId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Id[5] = 0x01;//开启蜂鸣器提示
	//Cmd_Read_Id[5] = 0x00;//关闭蜂鸣器提示
	TxCheckSum(Cmd_Read_Id,Cmd_Read_Id[1]);		//计算校验和
	esp8266_send_data("4",50);
//	Uart1_Send_Data(Cmd_Read_Id,Cmd_Read_Id[1]);		 //发送读卡号ID命�
	esp8266_send_data("5",50);
//	esp8266_send_data("pipixia",50);
//	esp8266_send_data(Cmd_Read_Id,Cmd_Read_Id[1]);
	delay_ms(150);//等待模块返回数据，大于150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//对接收到的数据校验
		esp8266_send_data("6",50);
		if(status != STATUS_OK)  //判断校验和是否正确
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK)	//判断是否正确的读到卡
		{
		 	return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa1))//判断是否为读卡号返回的数据包
		{
			for(i=0;i<6;i++)//获取卡号ID，6字节		 
			{
				idout[i] = Uart1RxBuf[i+5];//从数组的第5个字节开始为卡号，长度为6字节
			}
			return STATUS_OK;		 //成功返回0
		}
 	} 
	return STATUS_ERR;			//失败返回1
}
//ReadId（）:读IC卡数据块
//参数：*idout，读取的数据保存到它所指向的存储空间
//参数：block，块号
//返回值：0：成功读取，1：读读取失败
unsigned char ReadDataFromBlock(unsigned char *dataout,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Block[4] = block;
	Cmd_Read_Block[5] = 0x01;//开启蜂鸣器提示
//	Cmd_Read_Block[5] = 0x00;//关闭蜂鸣器提示
	TxCheckSum(Cmd_Read_Block,Cmd_Read_Block[1]);	//数据校验
	esp8266_send_data("pipixia",50);
	Uart1_Send_Data(Cmd_Read_Block,Cmd_Read_Block[1]);		 //发送读数据块命令
	delay_ms(150);//等待模块返回数据，大于150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//对接收到的数据校验
		if(status != STATUS_OK)		 //判断校验和是否正确
		{
			return 	STATUS_ERR;
		}
		status = Uart1RxBuf[4];		//获取返回包状态
		if(status != STATUS_OK)	//判断是否正确的读到卡
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa3))//判断是否为读块数据返回的数据包
		{
			for(i=0;i<16;i++)//获取块数据，16字节	，一个数据块的大小为16字节	 
			{
				dataout[i] = Uart1RxBuf[i+5];//从数组的第5个字节开始为数据，长度为16字节
			}
			return STATUS_OK;		 //成功返回0
		}
	}
	return STATUS_ERR;			//失败返回1
}
//ReadId（）:写数据到指定的数据块
//参数：*idout，指向要写入数据的缓冲区
//参数：block，块号
//返回值：0：写入成功，1：写入失败
unsigned char WriteDataToBlock(unsigned char *datain,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Write_Block[4] = block;
	for(i=0;i<16;i++)
	{
		Cmd_Write_Block[6+i] = datain[i];
	}
	TxCheckSum(Cmd_Write_Block,Cmd_Write_Block[1]);	//数据校验
	Uart1_Send_Data(Cmd_Write_Block,Cmd_Write_Block[1]);		 //发送写命令
	delay_ms(150);//等待模块返回数据，大于150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//对返回的数据进行校验
		if(status != STATUS_OK) //判断校验是否通过
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK) //判断校验是否通过
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa4))//判断是否为写块数据返回的数据包
		{
				return STATUS_OK;		 //成功返回0
		}
 	} 
	return STATUS_ERR;			//失败返回1
}