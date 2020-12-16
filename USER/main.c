#include "led.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "common.h" 
#include "touch.h"	
#include "usart2.h"	
#include "esp8266.h"
#include "Time_test.h"
#include "motor.h"
#include "stm32f10x.h"
#include "interface.h"
#include "IRCtrol.h"
#include "motor.h"
#include "nfc.h"
extern int Velocity_KP,Velocity_KI;	
unsigned char u3_rev[50];
unsigned char flag_usart3_rev=0;
u8 Data_buff[1024];
u16 n;
u32 indata1;
u32 indata2;
u32 indata;

//nfc
unsigned char Uart1RxBuf[UART1_RX_BUF_LEN];
unsigned char Uart1RxDataConut = 0x08;
unsigned char Rx1Flag = 1;

unsigned char Uart2RxBuf[UART2_RX_BUF_LEN];
unsigned char Uart2RxDataConut = 0;
unsigned char Rx2Flag = 1;

unsigned char Cmd_Read_Id[8] = {0x01,0x08,0xa1,0x20,0x00,0x00,0x00,0x00};
unsigned char Cmd_Read_Block[8]	= {0x01,0x08,0xa3,0x20,0x00,0x00,0x00,0x00};
unsigned char Cmd_Write_Block[23] = {0x01,0x17,0xa5,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char WBlockData[16] = {0x11,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char WBlockData[16];
extern int speed_left,speed_right;		   /*speed=0~5*/
volatile u32 time; // ms ��ʱ����
extern volatile u8 motor0_angel,motor1_angel,motor2_angel,motor3_angel,motor4_angel,motor5_angel;
u8 mode[3],mode1=0;
//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
short front_left_speed_duty=SPEED_DUTY;
short front_right_speed_duty=SPEED_DUTY;
short behind_left_speed_duty=SPEED_DUTY;
short behind_right_speed_duty=SPEED_DUTY;
unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ
char ctrl_comm = COMM_STOP;//����ָ��
unsigned char continue_time=0;
unsigned char duoji_count=0;
unsigned char zhuanjiao = 11;

//��ʱ��������0.1ms 
//���PWM������20ms��0 - 180��ֱ��Ӧ 0.5 - 2.5ms ������
//����zhuanjiao�ɵ���ΧΪ 5 - 25 ��Ӧ 0 - 180�㣬��zhuanjiao = 15 ʱ��Ӧ90��
//Ϊ���ö�����и���ȷ���������һ�����ת�ǲ�Ҫʹ��0���180�㣬�������м俿��һ��
//��תʱѡ��zhuanjiao=7 ��תʱ��zhuanjiao=23
void DuojiMid()
{
	zhuanjiao = 14;
}

void DuojiRight()
{
	zhuanjiao = 23;
}

void DuojiLeft()
{
	zhuanjiao = 7;
}


 int main(void)
 { 
	u8 i;
	u16 len;
	unsigned long long  timetick = 0;
	u32 pipixia = 0;
	unsigned char status;
	unsigned char  pkt[12];
	unsigned char id[4];
	unsigned char blockdata[16];
	unsigned char cmd[20]="pipixia";
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600	
	USART2_Init(115200);  //��ʼ������2������Ϊ115200
	SystemInit();					//��ʼ��ϵͳ
	NVIC_Configuration(); //��ʼ������ж�
	//COM1_2_Init();  //��ʼ��UART1,UART2
	delay_init();
	Rx1Flag = 0;
	Rx2Flag = 0;
	Uart1RxDataConut = 0x00;
	Uart2RxDataConut = 0;
	esp8266_quit_trans();
	Delayms(100);
	esp8266_start_trans();  
	esp8266_send_data(cmd,50);
	//START_TIME;	
	SystemInit();	
	GPIOCLKInit();

	TIM2_Init();
	MotorInit();
	DuojiMid();
	ServoInit();


	while(1)
	{   
//���İ�����
		if(timetick>=60000000000)
		{
			timetick = 0;
			 __set_FAULTMASK(1); 
			NVIC_SystemReset(); 
		}
		timetick++;
		//rfidģ���ȡ����
		if(Rx1Flag == 0x01)
		{
			Rx1Flag = 0x00;
			for(i = 0 ; i < 12;i++)
			{
				pkt[i] = Uart1RxBuf[i];
			}
//			Uart2_Send_Data(pkt,12);
			//04 0C 02 20 00 04 00 25 01 4A 3C 83 
			if((pkt[0] == 0x04)&&(pkt[1] == 0x0C)&&(pkt[2] == 0x02)&&(pkt[3] == 0x20))
			{
				for(i = 0 ; i < 4;i++)
				{
					id[i] = pkt[7 + i];
				}
				Uart2_Send_Data(id,4);//��仰�ǣ������Ӧ����id�����ҾͰ�id���Ļ�����Ϣ����������
			}
		}

		
		 if(USART2_RX_STA&0X8000)//���Ҳ��esp8266������Ϣ
			{
				len=USART2_RX_STA&0X7FFF;
				memcpy(Data_buff,USART2_RX_BUF,len);
				USART2_RX_STA=0;
				Data_buff[len]='\0';

			}
			if(strcmp(Data_buff, "0")==0)
			{
				CarStop();
			}
			else if(strcmp(Data_buff, "1")==0)
			{
				CarGo();
			}
			else if(strcmp(Data_buff, "2")==0)
			{
				CarLeft();
			}
			else if(strcmp(Data_buff, "3")==0)
			{
				CarRight();
			}
			else if(strcmp(Data_buff, "4")==0)
			{
				CarBack();
			}
			else if(strcmp(Data_buff, "5")==0)
			{
				CarRightFront();
			}
			else if(strcmp(Data_buff, "6")==0)
			{
				CarLeftFront();
			}
			else if(strcmp(Data_buff, "7")==0)
			{
				DuojiLeft();
			}
			else if(strcmp(Data_buff, "8")==0)
			{
				DuojiMid();
			}
	}
}























