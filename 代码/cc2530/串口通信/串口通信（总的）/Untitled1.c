
/*
#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
char flag,temp,counter=0;
char data[]="是谁？";
char name[20];

void init_LED(){
  P1SEL &=0xE6;
  
  P1DIR |=0x1B;
  LED1=1;
  LED2=1;
}

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

void delay(int i){
  int j,k;
  for(k=0;k<i;k++){
    for(j=0;j<500;j++);
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


void main(){
  init();
  init_LED();
  uart(data,sizeof(data));
  while(1){
    if(flag == 1)
    { flag = 0;
      if(temp != '#'){name[counter++]=temp;}
      else{
        U0CSR &= ~0x40;
        
        uart(" is  ",sizeof("shabi is  "));
      
        delay(1000);
        
        uart(name,sizeof(name));
        counter=0;
        U0CSR|=0x40;
      
      
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
*/

#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
char flag,temp,counter=0;
char data[]="谁是猪？";
char name[20];
int flagn=0;

void init_LED(){
  P1SEL &=0xE6;
  
  P1DIR |=0x1B;
  LED1=1;
  LED2=1;
  
  IEN2 |= 0x10;           
  P1IEN |= 0x04;         
  PICTL |= 0x02;        
  EA = 1;
  
  P1SEL &= ~0x1f;         
  P1DIR |= 0x1b;  
}

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

void delay(int i){
  int j,k;
  for(k=0;k<i;k++){
    for(j=0;j<500;j++);
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


void main(){
  init();
  init_LED();
  int la=1;
  while(1){
    switch(flagn){
    case 1:
      switch(la){
      case 1:uart(data,sizeof(data));la++;break;
      case 2: 
              if(flag == 1)
              { flag = 0;
                if(temp != '#'){name[counter++]=temp;}
                else{
                  U0CSR &= ~0x40;
                  uart("猪是 ",sizeof("猪是 "));
                  delay(1000);
                  uart(name,sizeof(name));
                  counter=0;
                  U0CSR|=0x40;
                  la=1;
                  break;
                }
              }
      }
            break;
      case 2:flag=0;break;
    }
  }
}

#pragma vector = URX0_VECTOR
__interrupt void ISR(void){
    URX0IF=0;
    temp = U0DBUF;
    flag=1;

}


#pragma  vector = P1INT_VECTOR
__interrupt void zd()
{
    if(P1_2)      
    {
        flagn++ ;
        LED1=0;
        P1IFG &= ~0x04;    
    }
    P1IF = 0;             
}