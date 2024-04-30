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
/*��Ե�ͨѶ��ַ����*/
#define RF_CHANNEL                16         // Ƶ�� 11~26  
#define PAN_ID                    0xACDC     //����id  
#define MY_ADDR                   0x0251     //����ģ���ַ
#define SEND_ADDR                 0x022F     //���͵�ַ
/* �Զ�����Ϣ��ʽ */
// #define START_HEAD    0xCC//֡ͷ
// #define CMD_READ      0x01//������������
// #define SENSOR_TEMP   0x01//�¶�
// #define SENSOR_RH     0x02//ʪ��
// #define SENSOR_FIRE   0x03//����
/*  LED n ��˸ time ���� �� */
int flag = 0;

#define FlashLed(n,time) do{\
                   halLedSet(n);\
                   halMcuWaitMs(time);\
                   halLedClear(n);\
                   }while(0)
/*�����С*/
#define MAX_SEND_BUF_LEN  2 //������������ͳ���
#define MAX_RECV_BUF_LEN  2 //�������������ճ���
/*����*/
static basicRfCfg_t basicRfConfig; 
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
uint8   APP_SEND_DATA_FLAG;  

/*****************************************************************************************
*������uint8 CheckSum(uint8 *buf, uint8 len) 
*���ܣ�����У��� 
*���룺uint8 *buf-ָ�����뻺����, uint8 len���������ֽڸ��� 
*������� 
*���أ�����У��� 
*����˵������ 
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
  basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������  
  basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������  
  basicRfConfig.myAddr      =   MY_ADDR;   //���ñ�����ַ  
  basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�  
  while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�  
  basicRfReceiveOn();                // ��RF  
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
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    Timer4_Init(); //��ʱ����ʼ��
    Timer4_On();  //�򿪶�ʱ��
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
