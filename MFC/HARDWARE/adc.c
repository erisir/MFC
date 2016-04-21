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
uint  ch=0, counter=10;
uint votage_input, key_input;
 

void  ADC_Init();  				// 初始化AD
void  Delay(unsigned int z); 	// 延时
void  view_Votage();
void  view_SetVotage();


void ADC_start()
{
	uint a = 0;
	
	PWM_Initial();
	PWM_clock(2);      // PCA/PWM时钟源为 定时器0的溢出
	PWM0_set(240); //0全开，作为对比
	while(1)
	{  
		view_Votage();
		view_SetVotage();
		a = KeyPro();  //********************************为后期键盘准备,目前只取key3 =0，key4=1
	 	if(a==0) KeyDown();
		if(a==1) KeyUp();		
	}
}  

void view_Votage()
{
	 uint K = 0;
	  
	votage_input =votage_input&0xff;
	K =   (uint)((4700l*votage_input)/255);
	compare_temper(K);
	in_str[1] = K / 1000;	  //最高位
	K = K % 1000;
	in_str[2] = K / 100;  		//次高位
	K = K % 100;
	in_str[3] = K /10;			//...
	in_str[4] = K % 10;			//....


	num_lcdDis(0,0x06,in_str[4],2);
	num_lcdDis(0,0x05,in_str[3],2);
	num_lcdDis(0,0x04,in_str[2],2);
	num_lcdDis(0,0x03,in_str[1],2);
	num_lcdDis(0,0x02,0,2);
 
 
}
void view_SetVotage()
{
	uint L = 0;

	L = getTemperSet();
	d[1] = L / 1000;	  //最高位
	L = L % 1000;
	d[2] = L / 100;  		//次高位
	L = L % 100;
	d[3] = L /10;			//...
	d[4] = L % 10;			//....
	
	num_lcdDis(1,0x06,d[4],2);
	num_lcdDis(1,0x05,d[3],2);
	num_lcdDis(1,0x04,d[2],2);
	num_lcdDis(1,0x03,d[1],2);
	num_lcdDis(1,0x02,d[0],2);


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
	 
	if (ch==0  )             
	 {
	 	votage_input= ADC_RES;
	 }
/*	else if(ch==1)
	{
	   votage1_input = 127;
	} */
    if(++ch>1)
    	ch=0;
 
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START|ch ;
}
