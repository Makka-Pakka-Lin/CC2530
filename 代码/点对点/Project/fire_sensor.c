#include "hal_defs.h"  
#include "hal_cc8051.h"  
#include "hal_int.h"  
#include "hal_mcu.h"  
#include "hal_board.h"  
#include "hal_led.h"  
#include "hal_adc.h"  
#include "hal_rf.h"  
#include "basic_rf.h"  
#include "hal_uart.h"   
#include "TIMER.h"  
#include "get_adc.h"  
#include "sh10.h"  
#include "UART_PRINT.h"  
#include "util.h"  
#include <stdlib.h>  
#include <string.h>  
#include <stdio.h>  
#include <math.h>  

#define button P1_2
/*点对点通讯地址设置*/
#define RF_CHANNEL                16         // 频道 11~26  
#define PAN_ID                    0xACDC     //网络id  
#define MY_ADDR                   0x0251     //本机模块地址
#define SEND_ADDR                 0x022F     //发送地址
/* 自定义消息格式 */
// #define START_HEAD    0xCC//帧头
// #define CMD_READ      0x01//读传感器数据
// #define SENSOR_TEMP   0x01//温度
// #define SENSOR_RH     0x02//湿度
// #define SENSOR_FIRE   0x03//火焰
/*  LED n 闪烁 time 毫秒 宏 */
int flag = 0;

#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)
/*数组大小*/
#define MAX_SEND_BUF_LEN  2 //无线数据最大发送长度
#define MAX_RECV_BUF_LEN  2 //无线数据最大接收长度
/*变量*/
static basicRfCfg_t basicRfConfig; 
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
uint8   APP_SEND_DATA_FLAG;  

/*****************************************************************************************
*函数：uint8 CheckSum(uint8 *buf, uint8 len) 
*功能：计算校验和 
*输入：uint8 *buf-指向输入缓存区, uint8 len输入数据字节个数 
*输出：无 
*返回：返回校验和 
*特殊说明：无 
*****************************************************************************************/ 
uint8 CheckSum(uint8 *buf, uint8 len)  
{  
  uint8 temp = 0;  
  while(len--)  
  {  
    temp += *buf;  
    buf++;  
  }  
  return (uint8)temp;  
}  


void ConfigRf_Init(void)  
{  
  basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置  
  basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置  
  basicRfConfig.myAddr      =   MY_ADDR;   //设置本机地址  
  basicRfConfig.ackRequest  =   TRUE;          //应答信号  
  while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功  
  basicRfReceiveOn();                // 打开RF  
}  

void init(){
	P1SEL &= ~0x1F;//00000000
	P1DIR |= ~0x04;
	P1DIR &= 0x1B;
  P1 = 0;
        
  IEN2 = 0x10;
  P1IEN = 0x04;
  PICTL = 0x02;
  EA = 1;
}
void main(void)    
{
    halBoardInit();  //模块相关资源的初始化
    ConfigRf_Init(); //无线收发参数的配置初始化 
    Timer4_Init(); //定时器初始化
    Timer4_On();  //打开定时器
    init();
    
    while(1)
    {   
      switch (flag)
      {
      case 1:
        pTxData[0]=0x01;
        basicRfSendPacket(
          (unsigned short)SEND_ADDR, 
          (unsigned char *)pTxData, 
          2);
        break;
      case 2:
        pTxData[0]=0x02;
        basicRfSendPacket(
          (unsigned short)SEND_ADDR, 
          (unsigned char *)pTxData, 
          2);
        break;
      case 3:
        pTxData[0]=0x03;
        basicRfSendPacket(
          (unsigned short)SEND_ADDR, 
          (unsigned char *)pTxData, 
          2);
        break;
      } 
    }
}
#pragma vector = 0x7b 
__interrupt void zd(void)
{
	if(!button){
    while(!button);
    flag++;
    if(flag == 3 ) flag = 0;	
}
	P1IFG = 0;
  P1IF = 0;
}
