#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
#define SW1 P1_2
#define LED3 P1_3
#define LED4 P1_4
unsigned char temp,flag,flagn;

void delay(int i){
  for(int l;l<i;l++){
    for(int k;k<500;k++);
  }
}
void init(){
  P1SEL &=~0x1F;
  P1DIR &=~0x04;
  P1DIR |=0x1B;
  P1IEN &=~0x04;
  P2IEN &=~0x40;
  P1 = 0;
  
  EA=1;
  IEN2 |=0x10;
  P1IEN |=0x04;
  PICTL |=0x02;
  
  CLKCONCMD &=0X80;
  while(CLKCONSTA & 0X40);
  PERCFG = 0X00;
  P0SEL |=0X3C;
  
  P2DIR &=~0XC0;
  U0CSR &=~0XC0;
  U0GCR =11;
  U0BAUD =216;
  URX0IF = 0;
  IEN0 = 0X84;
}
void uart(char *data,int len){
  unsigned int j;
  for(j=0;j<len;j++){
    U0DBUF = *data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
}
#pragma vector = URX0_VECTOR
__interrupt void UART0_RX_ISR(void){
  URX0IF = 0;
  temp =U0DBUF;
  flag =1;
}

#pragma vector = 0x7b
__interrupt void P1_ISR(void){
  if(P1_2){
    flagn++;
    switch(flagn){
    case 1: LED1=1;LED2=1;delay(500);
            LED1=0;LED2=0;break;
            
    case 2: LED1=1;LED2=1;delay(500);
            LED1=0;LED2=0;delay(500);
            LED1=1;LED2=1;delay(500);
            LED1=0;LED2=0;break;
    case 3:flagn=1;break;
    }
  }
  P1IF = 0x00;
  P1IFG = 0x00;
}

void main(){
  init();
  
}