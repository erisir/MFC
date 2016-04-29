/*
在中断中读取votage_input值，100ms一次，在adc_start中实现循环，pid控制
*/
#include "stc12c5a60s2.h"
#include "intrins.h"
#include "adc.h"
#include "PID.h"
#include "key.h"
#include "pwm.h"
#include "lcd1602.h"


#define U8 unsigned char
#define uint unsigned int
#define uchar unsigned char


#define ADC_POWER   0x80            //ADC power control bit 电源控制位
#define ADC_FLAG    0x10            //ADC complete flag   标志位
#define ADC_START   0x08            //ADC start control bit 启动控制位
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

unsigned char in_str[]="0.00V";
unsigned char d[5];
uint  ch=0, counter=0,counterLCD=0;getVoltageCounter=5;

uint votage_input,votage_tmp,votage_tmp1=2000, key_input;

 
void ADC_start()
{

 
	while(1)
	{   
	    uint i = 0;
		votage_tmp = 0;
	    for(i=0;i< getVoltageCounter ;i++){
		    votage_input = votage_input&0xff;
		    votage_tmp +=  (uint)((4700l*votage_input)/255)/getVoltageCounter;
			DelayMs(10);
		}
 
	    compare_temper(votage_tmp);	
		  
		DelayMs(400);


		num_lcdDis(0,0x02,getPIDSet_point(),4);
		num_lcdDis(1,0x02,votage_tmp,4);

		key_input = KeyPro();  //********************************为后期键盘准备,目前只取key3 =0，key4=1
	 	if(key_input==0) SetPointDown();
		if(key_input==1) SetPointUp();
	 		
	}
}
  
 


void ADC_Init()
{

    ADC_RES = 0;                                           //清除上次采集的结果	
	IE=0xa0;
	P1ASF=0xe0;                                            //设置AD输入通道为 P1.0-1.2 
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START|ch ;   //选择开启的通道
    Delay(2);                                              //上电启动AD转换的延时
}


void Delay(unsigned int n)
{
	unsigned int x;
    while (n--)
		{
			  x=5000;
			  while (x--);
		}
}


void ADC_isr(void) interrupt 5 using 1   //中断服务程序,每100ms读一次AD
{

	ADC_CONTR &= !ADC_FLAG; 
	if (counter==2)
	{
		if (ch==0  )             
		 {
		 	votage_input= ADC_RES;
		 }
		else if(ch==1)
		{
		  
			//light_input = 127;
		}
	  //else if (ch==2)  { key_input= ADC_RES; }
	    if(++ch>1)
	    	ch=0;

		counter = 0;
	}
	        
	counter++;
	counterLCD++;
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START|ch ;
}
