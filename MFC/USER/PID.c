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
              PID����
*************************************************/ 
void PIDInit (struct PID *pp) 
{ 
  memset ( pp,0,sizeof(struct PID)); 
} 
/************************************************
              ��������PID������ 
51��Ƭ����ó����������㣬ת����int�ͼ���
*************************************************/
unsigned int PIDCalc( struct PID *pp, unsigned int NextPoint ) 
{ 
  unsigned int dError,Error,pError; 
  //���������㹫ʽ��
  //Pdt=Kp*[E(t)-E(t-1)]+Ki*E(t)+Kd*[E(t)-2*E(t-1)+E(t-2)]
  Error = set_temper - NextPoint;       // ƫ��E(t) 
  pError=Error-pp->LastError;	 //E(t)-E(t-1)
  dError=Error-2*pp->LastError+pp->PrevError; //E(t)-2*E(t-1)+E(t-2) 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 
  return ( 
            pp->Proportion * pError        //���� 
            + pp->Integral *Error  //������ 
            + pp->Derivative * dError	  // ΢����
			);  
} 

/************************************************
				PID������ʼ��
*************************************************/
void PIDBEGIN() 
{ 
  PIDInit(&spid); // Initialize Structure 
  spid.Proportion = 10; // Set PID Coefficients 
  spid.Integral = 5; 
  spid.Derivative =4; 
}
/*********************************************************** 
             PID�¶ȿ�����������
***********************************************************/ 

void compare_temper(unsigned int v_input) 		//PID�¶ȿ����������
{ 
  unsigned char i; 
  temper = 	   v_input;
  if(set_temper>temper) 
   { 
    if(set_temper-temper>50)//�������Ŀ���¶�����ʵʱ�¶Ȳ����5�ȣ����Ŵ�10������50
     { 
       
	  // PWM0_set(10); //PWM ����ߵ�ƽռ�ձ���󡣼�ȫ�ټ���
	   PWM1_set(10);
     }
	else 
    { 
     for(i=0;i<10;i++) //5�ȷ�Χ��PID�������ƣ�10������ ��PID����ʽ��T=10
     { 
      rin=v_input;//PID����ʵʱ�¶Ȳ���ֵ
      rout = PIDCalc ( &spid,rin ); // PID������� 
     }//PID���������Χ��0-255�����pwmȡֵ��Χ 
     send_string_com("pid out:"); //��������������� 
	 send_char_com(rin); //ʵʱ�¶�ֵ
	 send_char_com(rout);//PID�������ֵ
	 send_char_com(0x0d);//����س������м�������ַ�
	 send_char_com(0x0a);//
	 
	 if(rout>=240)rout=240;	//����PWM����ֵ10-240 ��ֹpwm����ʧ����stc��Ƭ�����ԡ�
	 if(rout<=20)rout=20;
	 
	 //PWM0_set(255-rout); 
	 PWM1_set(255-rout);
	  
    } 
   } 
  else if(set_temper<=temper) //Ŀ���¶�С��ʵʱ�¶�pwm����͵�ƽ���ռ�ձȣ��رռ��ȡ�
   { 
     //PWM0_set(250); 
	 PWM1_set(250);
   } 
}