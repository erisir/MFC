/********************************************************************
������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
����ṹ�ο� ����ʦ����ѧ  Lyzhangxiang��EasyHW OS�ṹ���
datacomm.h
���ߣ�bg8wj
��������: 2012.12.23
�汾��V1.0

Copyright(C) bg8wj
/********************************************************************/
#ifndef  __PID_H__
#define  __PID_H__


/************************************************
PID����
*************************************************/ 
/*************PID**********************************/
struct PID {
float Proportion; // �������� Proportional Const
float Integral; // ���ֳ��� Integral Const
float Derivative; // ΢�ֳ��� Derivative Const

int LastError; // Error[-1]
int PrevError; // Error[-2]
int SumError;
int maxMoveStep;
int rout;
unsigned int set_point;

};

void PIDInit ();  
int PIDCalc( struct PID *pp, unsigned int NextPoint ) ;
void compare_temper(unsigned int votage_input); 		//PID�¶ȿ����������

unsigned int getPIDSet_point();
void SetPointDown (); 
void SetPointUp ();
#endif
