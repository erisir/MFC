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
float Proportion; // 比例常数 Proportional Const
float Integral; // 积分常数 Integral Const
float Derivative; // 微分常数 Derivative Const

int LastError; // Error[-1]
int PrevError; // Error[-2]
int SumError;
int maxMoveStep;
int rout;
unsigned int set_point;
unsigned int deadZone;
unsigned int duration;

};

void PIDInit ();  
int PIDCalc( struct PID *pp, unsigned int NextPoint ) ;
void compare_temper(unsigned int votage_input); 		//PID温度控制输出函数

unsigned int getPIDSet_point();
void SetPointDown (); 
void SetPointUp ();
void PIDparam_P_inc(); 
void PIDparam_P_dec();
void PIDparam_I_inc(); 
void PIDparam_I_dec();
void PIDparam_D_inc(); 
void PIDparam_D_dec();
void PIDparam_Dura_inc();
void PIDparam_Dura_dec();
#endif
