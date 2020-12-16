#include "nfc.h"
#include "delay.h"
#include "esp8266.h"

void COM1_2_Init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;	//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ģʽ

	/* ����GPIOʱ�� */
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
//����ʸ���жϣ�ʸ������˼������˳�����Ⱥ����˼��
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure1;	//�������ݽṹ��
  NVIC_InitTypeDef NVIC_InitStructure2;	//�������ݽṹ��
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//���ж�ʸ���ŵ�Flash��0��ַ


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�������ȼ����õ�ģʽ���������Ķ�ԭ�����е�����

  //ʹ�ܴ���1�жϣ����������ȼ�
  NVIC_InitStructure1.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure1);	//���ṹ�嶪�����ú�������д�뵽��Ӧ�Ĵ�����

    //ʹ�ܴ���2�жϣ����������ȼ�
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure2);	//���ṹ�嶪�����ú�������д�뵽��Ӧ�Ĵ�����

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

//ReadId����:��IC��ID�ţ����ţ�
//������*idout����ȡ�Ŀ��ű��浽����ָ��Ĵ洢�ռ�
//����ֵ��0���ɹ���ȡ���ţ�1��������ʧ��
unsigned char ReadId(unsigned char *idout)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Id[5] = 0x01;//������������ʾ
	//Cmd_Read_Id[5] = 0x00;//�رշ�������ʾ
	TxCheckSum(Cmd_Read_Id,Cmd_Read_Id[1]);		//����У���
	esp8266_send_data("4",50);
//	Uart1_Send_Data(Cmd_Read_Id,Cmd_Read_Id[1]);		 //���Ͷ�����ID���
	esp8266_send_data("5",50);
//	esp8266_send_data("pipixia",50);
//	esp8266_send_data(Cmd_Read_Id,Cmd_Read_Id[1]);
	delay_ms(150);//�ȴ�ģ�鷵�����ݣ�����150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//�Խ��յ�������У��
		esp8266_send_data("6",50);
		if(status != STATUS_OK)  //�ж�У����Ƿ���ȷ
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK)	//�ж��Ƿ���ȷ�Ķ�����
		{
		 	return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa1))//�ж��Ƿ�Ϊ�����ŷ��ص����ݰ�
		{
			for(i=0;i<6;i++)//��ȡ����ID��6�ֽ�		 
			{
				idout[i] = Uart1RxBuf[i+5];//������ĵ�5���ֽڿ�ʼΪ���ţ�����Ϊ6�ֽ�
			}
			return STATUS_OK;		 //�ɹ�����0
		}
 	} 
	return STATUS_ERR;			//ʧ�ܷ���1
}
//ReadId����:��IC�����ݿ�
//������*idout����ȡ�����ݱ��浽����ָ��Ĵ洢�ռ�
//������block�����
//����ֵ��0���ɹ���ȡ��1������ȡʧ��
unsigned char ReadDataFromBlock(unsigned char *dataout,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Read_Block[4] = block;
	Cmd_Read_Block[5] = 0x01;//������������ʾ
//	Cmd_Read_Block[5] = 0x00;//�رշ�������ʾ
	TxCheckSum(Cmd_Read_Block,Cmd_Read_Block[1]);	//����У��
	esp8266_send_data("pipixia",50);
	Uart1_Send_Data(Cmd_Read_Block,Cmd_Read_Block[1]);		 //���Ͷ����ݿ�����
	delay_ms(150);//�ȴ�ģ�鷵�����ݣ�����150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//�Խ��յ�������У��
		if(status != STATUS_OK)		 //�ж�У����Ƿ���ȷ
		{
			return 	STATUS_ERR;
		}
		status = Uart1RxBuf[4];		//��ȡ���ذ�״̬
		if(status != STATUS_OK)	//�ж��Ƿ���ȷ�Ķ�����
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa3))//�ж��Ƿ�Ϊ�������ݷ��ص����ݰ�
		{
			for(i=0;i<16;i++)//��ȡ�����ݣ�16�ֽ�	��һ�����ݿ�Ĵ�СΪ16�ֽ�	 
			{
				dataout[i] = Uart1RxBuf[i+5];//������ĵ�5���ֽڿ�ʼΪ���ݣ�����Ϊ16�ֽ�
			}
			return STATUS_OK;		 //�ɹ�����0
		}
	}
	return STATUS_ERR;			//ʧ�ܷ���1
}
//ReadId����:д���ݵ�ָ�������ݿ�
//������*idout��ָ��Ҫд�����ݵĻ�����
//������block�����
//����ֵ��0��д��ɹ���1��д��ʧ��
unsigned char WriteDataToBlock(unsigned char *datain,unsigned char block)
{
	unsigned char status;
	unsigned char i;
	Cmd_Write_Block[4] = block;
	for(i=0;i<16;i++)
	{
		Cmd_Write_Block[6+i] = datain[i];
	}
	TxCheckSum(Cmd_Write_Block,Cmd_Write_Block[1]);	//����У��
	Uart1_Send_Data(Cmd_Write_Block,Cmd_Write_Block[1]);		 //����д����
	delay_ms(150);//�ȴ�ģ�鷵�����ݣ�����150MS
 	if(Rx1Flag == 1)
 	{	
		Rx1Flag = 0;
		status = RxCheckSum(Uart1RxBuf,Uart1RxBuf[1]);//�Է��ص����ݽ���У��
		if(status != STATUS_OK) //�ж�У���Ƿ�ͨ��
		{
			return STATUS_ERR;
		}
		status = Uart1RxBuf[4];
		if(status != STATUS_OK) //�ж�У���Ƿ�ͨ��
		{
			return STATUS_ERR;
		}
		if((Uart1RxBuf[0] == 0x01)&&(Uart1RxBuf[2] == 0xa4))//�ж��Ƿ�Ϊд�����ݷ��ص����ݰ�
		{
				return STATUS_OK;		 //�ɹ�����0
		}
 	} 
	return STATUS_ERR;			//ʧ�ܷ���1
}