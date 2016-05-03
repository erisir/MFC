#include <reg52.h>
#include "lcd1602.h"
#include "pwm.h"
#include "key.h"
#include "adc.h"
#include "pid.h"
#include "Uart1.h"
//#include "STC12C5A60S2.h"
//#include <intrins.h>

#define U8 unsigned char
#define U16 unsigned int



unsigned   char   buf[8];
char rec;
char i;
char flag;//buf:@XX 1234
/****************************Ӳ���ӿڶ���***********************************
  PWM:ģ��0��ӦP1.3,ģ��1��ӦP1.4  [0Ϊ�ֶ����ƿڣ�1λPID�������ƿڡ�
  lcd:sbit RS = P2^3; sbit RW = P2^4; sbit E  = P2^5;  DBPort  P0 //���ݶ˿�
  adc:P1^0,P1^1��·�ɼ�
  key:KeyPort P3���Ժ���չʹ�á� ��sbit KEY3	= P2^6;   KEY4	= P2^7;
****************************************************************************/


void main(void)
{
	EA=1;//�����ж�
	lcd1602_init();
	hz_lcdDis(0,4,"Welcome!");
	
	InitUart1();
	PIDInit();
	//ADC_Init();	
	PWM_Initial();
 
	DelayMs(250);	
	lcd_Write_com(0x01);	//����
	hz_lcdDis(0,0,"S      mV Q ");
	hz_lcdDis(1,0,"G      mV d");
	
	PWM_clock(2);      // PCA/PWMʱ��ԴΪ ��ʱ��0�����

	ADC_start();

 }
/*------------------------------------------------*/
void serial () interrupt 4
{ 
send_string_com("hello!\r\n");
	if(RI) {		
		rec=SBUF;
		send_char_com(rec);			
		if(rec == '@'){//begin
			flag = 1;	
		}	
		if(flag ==1){
			buf[i] = rec;
			i++;	
			
			if(i==7){ 
			flag = 0;
			i=0;
			buf[7] = '\0';
			keyFunction(buf[1]);			
			}

		}
	 	RI=0;
	}

}
 