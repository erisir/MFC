/*
���ж��ж�ȡvotage_inputֵ��100msһ�Σ���adc_start��ʵ��ѭ����pid����
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


#define ADC_POWER   0x80            //ADC power control bit ��Դ����λ
#define ADC_FLAG    0x10            //ADC complete flag   ��־λ
#define ADC_START   0x08            //ADC start control bit ��������λ
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks

unsigned char in_str[]="0.00V";
unsigned char d[5];
uint  ch=0, counter=10;
uint votage_input, key_input;
 

void  ADC_Init();  				// ��ʼ��AD
void  Delay(unsigned int z); 	// ��ʱ
void  view_Votage();
void  view_SetVotage();


void ADC_start()
{
	uint a = 0;
	
	PWM_Initial();
	PWM_clock(2);      // PCA/PWMʱ��ԴΪ ��ʱ��0�����
	PWM0_set(240); //0ȫ������Ϊ�Ա�
	while(1)
	{  
		view_Votage();
		view_SetVotage();
		a = KeyPro();  //********************************Ϊ���ڼ���׼��,Ŀǰֻȡkey3 =0��key4=1
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
	in_str[1] = K / 1000;	  //���λ
	K = K % 1000;
	in_str[2] = K / 100;  		//�θ�λ
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
	d[1] = L / 1000;	  //���λ
	L = L % 1000;
	d[2] = L / 100;  		//�θ�λ
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

    ADC_RES = 0;                                           //����ϴβɼ��Ľ��	
	IE=0xa0;
	P1ASF=0xe0;                                            //����AD����ͨ��Ϊ P1.0-1.2 
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START|ch ;   //ѡ������ͨ��
    Delay(2);                                              //�ϵ�����ADת������ʱ
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


void ADC_isr(void) interrupt 5 using 1   //�жϷ������,ÿ100ms��һ��AD
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
