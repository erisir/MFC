#include <reg52.h>
#include "lcd1602.h"
#include "pwm.h"
#include "key.h"
#include "adc.h"
//#include "STC12C5A60S2.h"
//#include <intrins.h>

#define U8 unsigned char
#define U16 unsigned int



/****************************硬件接口定义***********************************
  PWM:模块0对应P1.3
  lcd:sbit RS = P2^3; sbit RW = P2^4; sbit E  = P2^5;  DBPort  P0 //数据端口
  adc:P1^0,P1^1两路采集
  key:KeyPort P3【以后扩展使用】 ，sbit KEY3	= P2^6;   KEY4	= P2^7;
****************************************************************************/


void main(void)
{
	EA=1;//开总中断
	lcd1602_init();
	hz_lcdDis(0,4,"Welcome!");
	DelayMs(250);	
	lcd_Write_com(0x01);	//清屏
	hz_lcdDis(0,0,"          mV");
	hz_lcdDis(1,0,"         PWM%");
	ADC_Init();
	ADC_start();
 }


