#include "PID.h"
#include "PWM.h"
#include "Uart1.h"
#include "string.h"

unsigned int rin;
unsigned int rout;
unsigned int set_temper=3000;
unsigned int temper;
xdata struct PID spid; // PID Control Structure

void KeyDown (){set_temper--;}
	 
void KeyUp (){set_temper++;} 
unsigned int getTemperSet(){return set_temper; }
/************************************************
              PID函数
*************************************************/ 
void PIDInit (struct PID *pp) 
{ 
  memset ( pp,0,sizeof(struct PID)); 
} 
/************************************************
              增量控制PID函数体 
51单片机最不擅长浮点数计算，转换成int型计算
*************************************************/
unsigned int PIDCalc( struct PID *pp, unsigned int NextPoint ) 
{ 
  unsigned int dError,Error,pError; 
  //增量法计算公式：
  //Pdt=Kp*[E(t)-E(t-1)]+Ki*E(t)+Kd*[E(t)-2*E(t-1)+E(t-2)]
  Error = set_temper - NextPoint;       // 偏差E(t) 
  pError=Error-pp->LastError;	 //E(t)-E(t-1)
  dError=Error-2*pp->LastError+pp->PrevError; //E(t)-2*E(t-1)+E(t-2) 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 
  return ( 
            pp->Proportion * pError        //比例 
            + pp->Integral *Error  //积分项 
            + pp->Derivative * dError	  // 微分项
			);  
} 

/************************************************
				PID函数初始化
*************************************************/
void PIDBEGIN() 
{ 
  PIDInit(&spid); // Initialize Structure 
  spid.Proportion = 10; // Set PID Coefficients 
  spid.Integral = 5; 
  spid.Derivative =4; 
}
/*********************************************************** 
             PID温度控制做动函数
***********************************************************/ 

void compare_temper(unsigned int v_input) 		//PID温度控制输出函数
{ 
  unsigned char i; 
  temper = 	   v_input;
  if(set_temper>temper) 
   { 
    if(set_temper-temper>50)//如果控制目标温度温与实时温度差大于5度，（放大10倍）是50
     { 
       
	  // PWM0_set(10); //PWM 输出高电平占空比最大。即全速加温
	   PWM1_set(10);
     }
	else 
    { 
     for(i=0;i<10;i++) //5度范围内PID增量控制，10次周期 即PID积分式中T=10
     { 
      rin=v_input;//PID输入实时温度采样值
      rout = PIDCalc ( &spid,rin ); // PID增量输出 
     }//PID增量输出范围（0-255）配合pwm取值范围 
     send_string_com("pid out:"); //串口输出监视数据 
	 send_char_com(rin); //实时温度值
	 send_char_com(rout);//PID增量输出值
	 send_char_com(0x0d);//输出回车，换行间隔控制字符
	 send_char_com(0x0a);//
	 
	 if(rout>=240)rout=240;	//保障PWM输入值10-240 防止pwm出现失调。stc单片机特性。
	 if(rout<=20)rout=20;
	 
	 //PWM0_set(255-rout); 
	 PWM1_set(255-rout);
	  
    } 
   } 
  else if(set_temper<=temper) //目标温度小于实时温度pwm输出低电平最高占空比，关闭加热。
   { 
     //PWM0_set(250); 
	 PWM1_set(250);
   } 
}