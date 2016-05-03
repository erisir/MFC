#include "PID.h"
#include "adc.h"

#include "PWM.h"
#include "Uart1.h"
#include "string.h"
#include "math.h"
 #include "lcd1602.h"

//xdata struct PID spid; // PID Control Structure
 struct PID spid; // PID Control Structure
 int pidCounter =0;

/************************************************
              PID函数体 
51单片机最不擅长浮点数计算，转换成int型计算
*************************************************/
int PIDCalc( struct PID *pp, unsigned int NextPoint ) 
{ 
  int Error,dError;
   
  Error = spid.set_point - NextPoint;       // 偏差E(t) 
  pp->SumError = pp->PrevError+pp->LastError+Error; 	                // 积分
  dError=Error - pp->LastError;             // 当前微分
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 

  if(abs(Error)< pp->deadZone){
  pp->SumError = 0;
  return 0;
  }

  return ( 
            pp->Proportion * Error        //比例 
            + pp->Integral * pp->SumError     //积分项 
            + pp->Derivative * dError	  // 微分项	 负责刹车
			);  
} 
 
/*********************************************************** 
             PID温度控制做动函数
***********************************************************/ 

void compare_temper(unsigned int v_input) 		//PID控制输出函数
{  
	 int delta = 0;
 	 pidCounter++;
	 if(pidCounter<spid.duration)return;
	 pidCounter = 0; 

	 delta =  PIDCalc ( &spid,v_input ); 

	 if( delta > spid.maxMoveStep){
	  delta =  spid.maxMoveStep;
	 }
	 if( delta < (-1*spid.maxMoveStep)){
	  delta =  (-1*spid.maxMoveStep);
	 }

     spid.rout += delta; 
	 

	 if(spid.rout>250)spid.rout = 250;

	 if(spid.rout< 5) spid.rout = 5;
 
	 
	 PWM1_set(spid.rout) ;
	 
	 num_lcdDis(0,0x0D,spid.rout,3);

	  if(delta>0){
	  hz_lcdDis(1,0x0C,"+");
	 	num_lcdDis(1,0x0D,delta,3);
	}else{
		hz_lcdDis(1,0x0C,"-");
		num_lcdDis(1,0x0D,-1*delta,3);
	}
	 	 
}
/************************************************
				PID函数初始化
*************************************************/
void PIDInit() 
{ 

  memset (&spid,0,sizeof(struct PID)); 	// Initialize Structure 
  
  spid.Proportion = 0.01; // Set PID Coefficients 	0.009开始震荡，取60%~70%=	0.006
  spid.Integral =   0.0002; 
  spid.Derivative =0.02; 
  spid.rout = 0;
  spid.set_point = 4000;
  spid.maxMoveStep = 80;
  spid.deadZone = 55;
  spid.duration = 4;
}

unsigned int getPIDSet_point()
{
	return spid.set_point; 
}
void SetPointDown ()
{
	spid.set_point-=1000;
	PWM0_set( spid.set_point/50);

}
 
void SetPointUp ()
{
	spid.set_point+=1000;
	PWM0_set(spid.set_point/50);

}
void PIDparam_P_inc ()
{
   spid.Proportion +=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Proportion*1000,2);
} 
void PIDparam_P_dec  ()
{
  spid.Proportion -=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Proportion*1000,2);
} 
void PIDparam_I_inc ()
{
   spid.Proportion +=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Proportion*1000,2);
}  
void PIDparam_I_dec  ()
{
   spid.Proportion -=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Integral*1000,2);
} 
void PIDparam_D_inc ()
{
   spid.Proportion +=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Derivative*1000,2);
}  
void PIDparam_D_dec  ()
{
  spid.Proportion -=0.001;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Derivative*1000,2);
}
void PIDparam_Dura_inc ()
{
   spid.duration +=5;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Derivative*1000,2);
}  
void PIDparam_Dura_dec  ()
{
  spid.duration -=5;
   hz_lcdDis(0,0x0a,"P");
   num_lcdDis(0,0x0b,spid.Derivative*1000,2);
}  