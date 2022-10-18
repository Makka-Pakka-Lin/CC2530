/* 包含头文件 */
#include "ioCC2530.h"
#include <stdio.h>
#define LED1 P1_0     // P1_0定义为P1_0  led灯端口
#define uint16 unsigned short
#define uint32 unsigned long



unsigned char SprintfAsciiResult[20];//存放转换后的ASCII字符串
unsigned char ProtocolBuff[9];//推送到网关的协议帧
uint16  counter=0; //统计溢出次数
unsigned char AsciiStr[8];//定义一个数组大小为8
unsigned char CHK(unsigned char *buf,int len);


void InitLED()
{
//考生完成LED的初始化
//=====》空白处开始
    P1SEL &=~ 0x01; 
    P1DIR |= 0x01;
    P1 = 0;
//《=====空白处结束
}

void adc_Init(void)
{
  APCFG  |=0x01;        //使能P0模拟外设功能，指定P0_0
  P0SEL  |=0x01;	//设置P0_0 为外设口
  P0DIR  &=~0x01;	//设置p0_0 为输入口
}

/************************************************************
* 名称       get_adc
* 功能       读取ADC通道0电压值
* 入口参数   无
* 出口参数   16位电压值，分辨率为10mV
***************获取ADC通道0电压值************************/

uint16 get_adc(void)
{
	uint32 value;
	ADCIF = 0;   //清ADC 中断标志
	//采用基准电压avdd5:3.3V，通道0，启动AD转化
	//考生完成ADC的初始化
//=====》空白处开始
    APCFG |= 0x01;
    P0SEL|= (1 << (0));
    P0DIR |=~ (1<<(0));
    ADCCON3 = 0x90;
    
    
    
//《=====空白处结束
	// AD值转化成电压值
	// 0 表示 0V ，32768 表示 3.3V
	// 电压值 = (value*3.3)/32768 （V)
    value = (value * 330);// 返回分辨率为10mV的电压值
    value = value >> 15;   // 除以32768
	// 返回分辨率为0.01V的电压值
    return (uint16)value;
}

/**********串口通信初始化************************/
void initUART0(void)
{
	PERCFG = 0x00;	
	P0SEL = 0x3c;	
	//由考生设置串口相关参数，要求波特率115200
    //=====》空白处开始
    CLKCONCMD &=~ 0x7F;
    while(CLKCONSTA & 0x40){
      CLKCONCMD &=~ 0x47;
      PERCFG = 0x00;
      P0SEL |= 0x3c;
      
      P2DIR &=~0xC0;
      U0CSR = 0x80;
      U0GCR = 11;
      U0BAUD = 216;
      UTX0IF = 0;
      U0UCR =0x00;
    }
    
    
    
    //《=====空白处结束
	UTX0IF = 0;  // 清零UART0 TX中断标志
	EA = 1;   //使能全局中断
}

/*************************************************
* 函数名称：initTimer1
* 功    能：初始化定时器T1控制状态寄存器
******************定时器初始化*****************************/
void initTimer1()
{
//考生完成LED的初始化
//=====》空白处开始
    LED1 = 0;
    
    
    
//《=====空白处结束
	
    T1IF=0;           //清除timer1中断标志(同IRCON &= ~0x02)
    T1STAT &= ~0x01;  //清除通道0中断标志
    TIMIF &= ~0x40;  //不产生定时器1的溢出中断
   //定时器1的通道0的中断使能T1CCTL0.IM默认使能
    IEN1 |= 0x02;    //使能定时器1的中断
    EA = 1;        //使能全局中断
}


/**************单片机发送协议帧数据到串口******************/
void UART0SendData(unsigned char *str,int len )
{
  for(int i=0;i<=len;i++)
  {
    U0DBUF = str[i];   // 将要发送的1字节数据写入U0DBUF
    while (!UTX0IF) ;  // 等待TX中断标志，即U0DBUF就绪
    UTX0IF = 0;        // 清零TX中断标志UART0SendByte(*str++); 
  }
}

/**************构建发送电压值到物联网网关的协议帧******************/
void BuildProtocolFrame(uint16 voltage)
{
  ProtocolBuff[0]=0xDD;
  ProtocolBuff[1]=0x03;
  ProtocolBuff[2]=0x00;
  ProtocolBuff[3]=0x01;
  ProtocolBuff[4]=0x09;
  ProtocolBuff[5]=0x04;
  ProtocolBuff[6]=voltage>>8;
  ProtocolBuff[7]=voltage&0xFF;
  ProtocolBuff[8]=CHK(ProtocolBuff,8);
}

/**************和校验函数******************/
 unsigned char CHK(unsigned char *buf,int len)
{
    unsigned char  RX_CHX=0;
     while(len--) {
        RX_CHX+= *buf;
        buf++;
    }

    return RX_CHX&=0xff;
}

/**************获取电压值并处理数据******************/
void Get_AsciiValue()
{
  uint16 sensor_val;
  sensor_val=get_adc();
  AsciiStr[0]=sensor_val/100+'0';
  AsciiStr[1]='.';
  AsciiStr[2]=sensor_val/10%10+'0';
  AsciiStr[3]=sensor_val%10+'0';
  AsciiStr[4]='V';
  AsciiStr[5]='\n';
 }

/******************************************
* 功    能：定时器T1中断服务子程序
************************************/
#pragma vector = T1_VECTOR //中断服务子程序
__interrupt void T1_ISR(void)
{
	EA = 0;   //禁止全局中断
	counter++;
	T1STAT &= ~0x01;  //清除通道0中断标志
	EA = 1;   //使能全局中断
}
//LED1闪烁时间延迟
void delay(int time)
{
   for(int i=0;i<=time;i++)
   {
     for(int j=0;j<=240;j++);
   }


}
/******************************************
* 函数名称：main
* 功    能：main函数入口
* 入口参数：无
* 出口参数：无
* 返 回 值：无
**************************************************/
void main(void)
{
    InitLED();
    initTimer1();  //初始化Timer1
    initUART0();  // UART0初始化
    adc_Init(); // ADC初始化
   while(1)
	{
	if(counter>=10)     //定时器每0.2S溢出中断计次
	{
	  counter=0;       //清标志位
	  LED1 = 1;    //指示灯点亮
	  Get_AsciiValue();
#ifdef debug
	//由考生完成向串口发送可燃气体值ASCII字符串
    //=====》空白处开始
    UART0SendData(Get_AsciiValue(),6);
    ADCCON3 = 0x90;
    
    
    
    
    //《=====空白处结束
#else
     //由考生完成向串口发送可燃气体协议帧
    //=====》空白处开始
    
    
    
    
    //《=====空白处结束
     delay(500);
                        
#endif
                        
    LED1 = 0;    //指示灯熄灭
      }
  }
}
