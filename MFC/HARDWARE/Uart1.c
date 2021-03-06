/******************************************************************
本程序只供学习使用，未经作者许可，不得用于其它任何用途
程序结构参考 安徽师范大学  Lyzhangxiang的EasyHW OS结构设计
uart1.C  file
作者：bg8wj
建立日期: 2011.12.23
版本：V1.0
Copyright(C) bg8wj
/*******************************************************************/
#include "Uart1.h"
#include "STC12C5A60S2.h"
#include "string.h"
#include "adc.h"


/************************************************************
				   uart1初始化
*************************************************************/
void InitUart1()
{
	TMOD = 0x20;    //定时器1工作在方式2  8位自动重装
    SCON = 0x50;    //串口1工作在方式1  10位异步收发 REN=1允许接收
    TH1 = 0xF3;	   //定时器1初值
    TL1 = TH1;
    TR1 = 1;	   //定时器1开始计数
    
    ES =1;	//开串口1中断

 
    
     
} 
/************************************************************
				   向串口uart1发送一个字符
*************************************************************/
void send_char_com(unsigned char ch)  
{
    SBUF=ch;
    while(TI==0);
    TI=0;
}
/************************************************************
	向串口uart1发送一个字符串，strlen为该字符串长度
*************************************************************/
void send_string_com(unsigned char *str)
{
    unsigned int k=0;
	
	do
    {
        send_char_com(*(str + k));
        k++;
    } while(k < strlen(str));
}

