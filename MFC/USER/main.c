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
/****************************硬件接口定义***********************************
  PWM:模块0对应P1.3,模块1对应P1.4  [0为手动控制口，1位PID反馈控制口】
  lcd:sbit RS = P2^3; sbit RW = P2^4; sbit E  = P2^5;  DBPort  P0 //数据端口
  adc:P1^0,P1^1两路采集
  key:KeyPort P3【以后扩展使用】 ，sbit KEY3	= P2^6;   KEY4	= P2^7;
****************************************************************************/


void main(void)
{
	EA=1;//开总中断
	lcd1602_init();
	hz_lcdDis(0,4,"Welcome!");
	
	InitUart1();
	PIDInit();
	//ADC_Init();	
	PWM_Initial();
 
	DelayMs(250);	
	lcd_Write_com(0x01);	//清屏
	hz_lcdDis(0,0,"S      mV Q ");
	hz_lcdDis(1,0,"G      mV d");
	
	PWM_clock(2);      // PCA/PWM时钟源为 定时器0的溢出

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
 