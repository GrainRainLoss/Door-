#ifndef __MOTOR_H_
#define __MOTOR_H_

extern unsigned int speed_count;//ռ�ձȼ����� 50��һ����
extern short front_left_speed_duty;
extern short front_right_speed_duty;
extern short behind_left_speed_duty;
extern short behind_right_speed_duty;

void CarMove(void);
void CarGo(void);
void CarBack(void);
void CarLeft(void);
void CarRight(void);
void CarLeftFront(void);
void CarRightFront(void);
void CarStop(void);
void MotorInit(void);
#endif

