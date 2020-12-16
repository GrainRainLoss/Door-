#ifndef __NFC_H
#define __NFC_H
#include "usart.h"
#define STATUS_OK			0x00
#define STATUS_ERR    0x01

extern unsigned char Uart1RxBuf[UART1_RX_BUF_LEN];
extern unsigned char Uart1RxDataConut;
extern unsigned char Rx1Flag;

extern unsigned char Uart2RxBuf[UART2_RX_BUF_LEN];
extern unsigned char Uart2RxDataConut;
extern unsigned char Rx2Flag;

extern unsigned char Cmd_Read_Id[8];
extern unsigned char Cmd_Read_Block[8];
extern unsigned char Cmd_Write_Block[23];

void COM1_2_Init( void);
void NVIC_Configuration(void);
void Uart1_Send_Data(unsigned char *buf,unsigned char num);
void Uart2_Send_Data(unsigned char *buf,unsigned char num);
unsigned char RxCheckSum(unsigned char *ptr,unsigned char len);
void TxCheckSum(unsigned char *ptr,unsigned char len);
unsigned char ReadId(unsigned char *idout);
unsigned char ReadDataFromBlock(unsigned char *dataout,unsigned char block);
unsigned char WriteDataToBlock(unsigned char *datain,unsigned char block);
#endif

