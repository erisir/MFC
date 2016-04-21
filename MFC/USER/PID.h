/********************************************************************
本程序只供学习使用，未经作者许可，不得用于其它任何用途
程序结构参考 安徽师范大学  Lyzhangxiang的EasyHW OS结构设计
datacomm.h
作者：bg8wj
建立日期: 2012.12.23
版本：V1.0

Copyright(C) bg8wj
/********************************************************************/
#ifndef  __PID_H__
#define  __PID_H__


/************************************************
PID函数
*************************************************/ 
/*************PID**********************************/
struct PID {
unsigned int Proportion; // 比例常数 Proportional Const
unsigned int Integral; // 积分常数 Integral Const
unsigned int Derivative; // 微分常数 Derivative Const
unsigned int LastError; // Error[-1]
unsigned int PrevError; // Error[-2]
};


/*unsigned int rout; // PID Response (Output) 
unsigned int rin; // PID Feedback (Input)
unsigned int temper;
unsigned char shu[3]={0,0,0};
unsigned char counter;

bit tp_flag;
unsigned char buff[8];
	*/

void PIDBEGIN();
unsigned int PIDCalc( struct PID *pp, unsigned int NextPoint ) ;
void PIDInit (struct PID *pp); 
void KeyDown (); 
void KeyUp (); 
void compare_temper(unsigned int votage_input); 		//PID温度控制输出函数
unsigned int getTemperSet();

#endif
