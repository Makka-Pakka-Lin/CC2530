#include "hal_defs.h"
#include "hal_cc8051.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "hal_led.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_uart.h" 
#include "hal_pwm.h" 
#include "UART_PRINT.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*****点对点通讯地址设置******/
#define RF_CHANNEL                16         // 频道 11~26
#define PAN_ID                    0xD0C2     //网络id
#define MY_ADDR                   0xB4F3     //本机模块地址

/******通讯协议相关*******/
#define START_HEAD    0xCC//帧头  
#define CMD_READ      0x01//读传感器数据  
#define SENSOR_TEMP   0x01//温度  
#define SENSOR_RH     0x02//湿度  
#define SENSOR_FIRE   0x03//火焰  

/********无线数据缓存********/
#define MAX_SEND_BUF_LEN  2
#define MAX_RECV_BUF_LEN  2
static uint8 pTxData[MAX_SEND_BUF_LEN]; //定义无线发送缓冲区的大小
static uint8 pRxData[MAX_RECV_BUF_LEN]; //定义无线接收缓冲区的大小
 
/**************************************************/
static basicRfCfg_t basicRfConfig;


//使LED n 闪烁 time 毫秒
#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)

void delay(int x){
	for(int y=x; y >0;y--){
		for(int z = 573; z>0;z--);
	}
}
/**********************************************************************************************
*函数：uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)
*功能：生成数组的16进制形式的字符串格式,成员间以空格隔开
*     例如:由{0xA1,0xB2,0xC3}生成"A1 B2 C3"
*输入：uint8 *input-指向输入缓存区, uint8 len-输入数据的字节长度
*输出：uint8 *output-指向输出缓存区
*返回：返回生成字符串的长度
*特殊说明：无
**********************************************************************************************/
uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)
{
  char str[128];
  memset(str, '\0', 128);
  for(uint8 i=0; i<len; i++)
  {
    sprintf(str+i*3,"%02X ", *input);
    input++;
  }
  strcpy((char *)output, (const char *)str);
  return strlen((const char *)str);
}

// 无线RF初始化
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee的ID号设置
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee的频道设置
    basicRfConfig.myAddr      =  MY_ADDR;   //设置本机地址
    basicRfConfig.ackRequest  =   TRUE;          //应答信号
    while(basicRfInit(&basicRfConfig) == FAILED); //检测zigbee的参数是否配置成功
    basicRfReceiveOn();                // 打开RF
}

/************************************************************************
*函数：uint8 CheckSum(uint8 *buf, uint8 len) 
*功能：计算校验和 
*输入：uint8 *buf-指向输入缓存区, uint8 len输入数据字节个数 
*输出：无 
*返回：返回校验和 
*特殊说明：无 
*************************************************************************/  
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



/********************MAIN START************************/
void main(void)
{
    uint16 len = 0;
    uint32 TimCnt = 0;
    int8 brightness = 0;
    uint8 flag = 0;//0-向下渐变暗，1-向上渐变亮
    
    halBoardInit();  //模块相关资源的初始化
    ConfigRf_Init(); //无线收发参数的配置初始化 
    
    while(1)
    {
    if(basicRfPacketIsReady())   //查询有没收到无线信号
      {
          //接收无线数据
          P0 = 0xff;
          len = basicRfReceive(pRxData, MAX_RECV_BUF_LEN, NULL);
         
    }
    else
    {
       switch (len[0])
           {
            case 0x01:
              P1 = 0x00;
            break;
            case 0x02:
              if(TimCnt++>1024)
                {

                  TimCnt = 0;
                  if(flag)
                    {

                      brightness ++;
                      if(brightness >= 90) flag = 0;//向下渐变暗
                    }
                    else
                    {

                      brightness --;
                      if(brightness <= 10) flag = 1;//向上渐变亮
                    }
                    TIM1_PwmInit(1,brightness);

                }
            break;
            case 0x03:
              P1 = 0x00;
              delay(2000);
              P1 = 0xff;
            break;

            // case 0x02:
            //   P0 = 0xFE;
            //   for(int x=0;x<8;x++)
            //   {
            //       P0<<=1;
            //       delay(1000);
            //   }
            // break;
          }

    }
}
/************************MAIN END ****************************/