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
              PID������ 
51��Ƭ����ó����������㣬ת����int�ͼ���
*************************************************/
int PIDCalc( struct PID *pp, unsigned int NextPoint ) 
{ 
  int Error,dError;
   
  Error = spid.set_point - NextPoint;       // ƫ��E(t) 
  pp->SumError = pp->PrevError+pp->LastError+Error; 	                // ����
  dError=Error - pp->LastError;             // ��ǰ΢��
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 

  if(abs(Error)< pp->deadZone){
  pp->SumError = 0;
  return 0;
  }

  return ( 
            pp->Proportion * Error        //���� 
            + pp->Integral * pp->SumError     //������ 
            + pp->Derivative * dError	  // ΢����	 ����ɲ��
			);  
} 
 
/*********************************************************** 
             PID�¶ȿ�����������
***********************************************************/ 

void compare_temper(unsigned int v_input) 		//PID�����������
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
				PID������ʼ��
*************************************************/
void PIDInit() 
{ 

  memset (&spid,0,sizeof(struct PID)); 	// Initialize Structure 
  
  spid.Proportion = 0.01; // Set PID Coefficients 	0.009��ʼ�𵴣�ȡ60%~70%=	0.006
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