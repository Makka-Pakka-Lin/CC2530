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


/*****��Ե�ͨѶ��ַ����******/
#define RF_CHANNEL                16         // Ƶ�� 11~26
#define PAN_ID                    0xD0C2     //����id
#define MY_ADDR                   0xB4F3     //����ģ���ַ

/******ͨѶЭ�����*******/
#define START_HEAD    0xCC//֡ͷ  
#define CMD_READ      0x01//������������  
#define SENSOR_TEMP   0x01//�¶�  
#define SENSOR_RH     0x02//ʪ��  
#define SENSOR_FIRE   0x03//����  

/********�������ݻ���********/
#define MAX_SEND_BUF_LEN  2
#define MAX_RECV_BUF_LEN  2
static uint8 pTxData[MAX_SEND_BUF_LEN]; //�������߷��ͻ������Ĵ�С
static uint8 pRxData[MAX_RECV_BUF_LEN]; //�������߽��ջ������Ĵ�С
 
/**************************************************/
static basicRfCfg_t basicRfConfig;


//ʹLED n ��˸ time ����
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
*������uint8 GetHexStr(uint8 *input, uint8 len, uint8 *output)
*���ܣ����������16������ʽ���ַ�����ʽ,��Ա���Կո����
*     ����:��{0xA1,0xB2,0xC3}����"A1 B2 C3"
*���룺uint8 *input-ָ�����뻺����, uint8 len-�������ݵ��ֽڳ���
*�����uint8 *output-ָ�����������
*���أ����������ַ����ĳ���
*����˵������
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

// ����RF��ʼ��
void ConfigRf_Init(void)
{
    basicRfConfig.panId       =   PAN_ID;        //zigbee��ID������
    basicRfConfig.channel     =   RF_CHANNEL;    //zigbee��Ƶ������
    basicRfConfig.myAddr      =  MY_ADDR;   //���ñ�����ַ
    basicRfConfig.ackRequest  =   TRUE;          //Ӧ���ź�
    while(basicRfInit(&basicRfConfig) == FAILED); //���zigbee�Ĳ����Ƿ����óɹ�
    basicRfReceiveOn();                // ��RF
}

/************************************************************************
*������uint8 CheckSum(uint8 *buf, uint8 len) 
*���ܣ�����У��� 
*���룺uint8 *buf-ָ�����뻺����, uint8 len���������ֽڸ��� 
*������� 
*���أ�����У��� 
*����˵������ 
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
    uint8 flag = 0;//0-���½��䰵��1-���Ͻ�����
    
    halBoardInit();  //ģ�������Դ�ĳ�ʼ��
    ConfigRf_Init(); //�����շ����������ó�ʼ�� 
    
    while(1)
    {
    if(basicRfPacketIsReady())   //��ѯ��û�յ������ź�
      {
          //������������
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
                      if(brightness >= 90) flag = 0;//���½��䰵
                    }
                    else
                    {

                      brightness --;
                      if(brightness <= 10) flag = 1;//���Ͻ�����
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