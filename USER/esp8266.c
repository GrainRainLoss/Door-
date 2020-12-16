#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart2.h"
#include "sys.h"
#include "interface.h"

//ESP8266ģ���PC����͸��ģʽ
void esp8266_start_trans(void)
{
	//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	esp8266_send_cmd("AT+CWMODE=1","OK",500);
	Delayms(800);
	//��Wifiģ������������
	printf("%d",esp8266_send_cmd("AT+RST","OK",500)); 
	Delayms(1000);         //��ʱ3S�ȴ������ɹ�
	Delayms(1000);
	Delayms(1000);
	
	//��ģ���������Լ���·��,   ANSI  CODE
	//while(esp8266_send_cmd("AT+CWJAP=\"飞机杯里泡枸杞_2.4G\",\"T1620183\"","WIFI GOT IP",800));
	while(esp8266_send_cmd("AT+CWJAP=\"飞机杯里泡枸杞_2.4\",\"T1620183\"","WIFI GOT IP",800));
	//=0����·����ģʽ     =1����·����ģʽ
	//esp8266_send_cmd("AT+CIPMUX=0","OK",20);
	Delayms(500);
	//����TCP����  ������ֱ������ Ҫ���ӵ�ID��0~4   ��������  Զ�̷�����IP��ַ   Զ�̷������˿ں�
	//while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"10.0.0.144\",8080","CONNECT",200));
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"118.24.57.221\",20183","CONNECT",200));
	//�Ƿ���͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
	esp8266_send_cmd("AT+CIPMODE=1","OK",500);
	Delayms(500);
	//͸��ģʽ�� ��ʼ�������ݵ�ָ�� ���ָ��֮��Ϳ���ֱ�ӷ�������
	esp8266_send_cmd("AT+CIPSEND","OK",500);
	printf("success!\r\n");
}

//ESP8266�˳�͸��ģʽ   ����ֵ:0,�˳��ɹ�;1,�˳�ʧ��
//����wifiģ�飬ͨ����wifiģ����������3��+��ÿ��+��֮�� ����10ms,������Ϊ���������η���+��
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	Delayms(500);
	u2_printf("+++");
	Delayms(2000);					//�ȴ�500ms̫�� Ҫ1000ms�ſ����˳�
	result=esp8266_send_cmd("AT","OK",500);//�˳�͸���ж�.
	if(result)
		printf("quit_trans failed!\r\n");
	else
		printf("quit_trans success!\r\n");
	return result;
}


//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			Delayms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//��ESP8266��������
//cmd:���͵������ַ���;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:�������ݺ󣬷������ķ�����֤��
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART2_RX_STA=0;
	u2_printf("%s",cmd);	//��������
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			Delayms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
				ack=(char*)USART2_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				USART2_RX_STA=0;
				break;//�õ���Ч���� 
			} 
		}
	}
	return (u8*)ack;
}

