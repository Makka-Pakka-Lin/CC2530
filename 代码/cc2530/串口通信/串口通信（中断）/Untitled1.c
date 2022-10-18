#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
char flag,temp;



void init(){ //输入和输出初始化都一样
  CLKCONCMD &=0x80;
  while(CLKCONSTA & 0x40);

  
  PERCFG = 0x00;
  P0SEL|=0x3C;
  
  P2DIR &=~0xC0;
  U0CSR = 0xC0; // 不一样！!!!
  U0GCR = 9;
  U0BAUD = 59;
  UTX0IF = 0;
  IEN0 = 0x84;
}


void main(){
  init();
  P1SEL &=0xE6;
  
  P1DIR |=0x1B;
  LED1=1;
  LED2=1;
  while(1){
    if(flag == 1)
    {
     flag = 0;
     switch(temp){
        case '1': LED1=0;break;
        case '2': LED2 =0;break;
        default:break;
     }
    }
  }
}

#pragma vector = URX0_VECTOR
__interrupt void ISR(void){
    URX0IF=0;
    temp = U0DBUF;
    flag=1;

}


/*
void main(){
  init();
  while(1){
    uart(data,sizeof(data));
    delay(1000);
  }
}


void uart(char *data,int len){
  int j;
  for(j=0;j<len;j++){
    U0DBUF = *data++;
    while(UTX0IF == 0);
    UTX0IF =0;
  }
}

///////输出//////


void delay(int i){
  int j,k;
  for(k=0;k<i;k++){
    for(j=0;j<500;j++);
  }
}
*/