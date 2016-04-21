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
unsigned int Proportion; // �������� Proportional Const
unsigned int Integral; // ���ֳ��� Integral Const
unsigned int Derivative; // ΢�ֳ��� Derivative Const
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
void compare_temper(unsigned int votage_input); 		//PID�¶ȿ����������
unsigned int getTemperSet();

#endif
