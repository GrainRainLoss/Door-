#ifndef _esp8266_H
#define _esp8266_H

//用户配置AT指令
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];


//函数

unsigned char esp8266_send_cmd(unsigned char *cmd,unsigned char *ack,unsigned short int waittime);
unsigned char* esp8266_send_data(unsigned char *cmd,unsigned short int waittime);
unsigned char* esp8266_check_cmd(unsigned char *str);
void esp8266_start_trans(void);
unsigned char esp8266_quit_trans(void);

#endif
