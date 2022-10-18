#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
#define SW1  P1_2
#define LED3 P1_3
#define LED4 P1_4
int flag =0;
int found =0;
int a=0;
int b=0;
int C=0;
void init(){
	P1SEL &= ~0x1F;
	P1DIR |= ~0x04;
	P1DIR &= 0x1B;
        P1 = 0;
        
        IEN2 = 0x10;
        P1IEN = 0x04;
        PICTL = 0x02;
        EA = 1;
        
}
void delay(int time)
{
  int i,j;
  for(i=0;i<30;i++)
  {
    for(j=0;j<time;j++);
  }
}

#pragma optimize=none
void ding(){
        CLKCONCMD &= ~0x7f;
        while(CLKCONSTA & 0x40);

        EA = 1;
        T1IE = 1;
        T1CTL =0x0F;
        T1CC0H = 0x0f;
        T1CC0L =0xff;
}
void sendByteByUart(char c)
{
  U0DBUF = c;
  while(!UTX0IF);
  UTX0IF=0;  
}
void sendStringByUart(char *str)
{
  while(*str != '\0')
  {
    sendByteByUart(*str);
    str++;
  }
}
void init_Uart()
{
  CLKCONCMD &= 0X80;
  
  //2.串口0的备用位置1
  PERCFG &=0xFE;//1111 1110 选中串口0的的备用位置1: P0_2 (RX), p0_3(TX)
  P0SEL  |=0x0C;       //0000 1100 串口使用P0_2 p0_3，配置为偏上外设功能
    
   U0UCR &=~(0x01<<2 |0x01<<3|0x01<<4);

   
   //由考生设置波特率19200
  //=====》空白处开始  
  U0BAUD |=59;
   
  //《=====空白处结束
   
   //设置寄存器为UART模式 :第7位为1
   U0CSR |=0X1<<7;
   
   //清发送中断标志 p41 IRCON2:第1位为0清中断
   UTX0IF=0; 
   EA=1; //开CPU中断
}
void main(){
  ding();
  init();
  init_Uart();
  while(1){
  
    
  } 
}
#pragma vector = T1_VECTOR
__interrupt void T1_ISR(void)
{ 
  switch(flag){
    case 0:LED1=1;
          LED2=0;
          P1_3=0;
          P1_4=0;
          break;
  
      case 1:if(found<20)
                {
                  found++;
                }
             else
               {
                  C++;
                  found=15;
               }
             switch(C){
                 case 1:P1_0 =1;P1_4 =0;break;
                 case 2:P1_1 =1;P1_0 =0;break;
                 case 3:P1_3 =1;P1_1 =0;break;
                 case 4:P1_4 =1;P1_3 =0;C=0;break;
             }
             break;
      case 2:
        C=0;
        found=0;
        P1=0;
         break;
  }
}

#pragma vector = 0x7b 
__interrupt void zd(void)
{
	if(!P1_2)
          {
            while(!P1_2);
            flag++;
            if(flag==3)
              {
                flag=1;
              }
          }
P1IFG = 0;
P1IF = 0;
}






