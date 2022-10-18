#include<ioCC2530.h>
#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_3
#define LED4 P1_4
#define SW1  P1_2

void delay(unsigned int i){
  int k,j;
  for(k<0;k<i;k++){
    for(j=0;j<500;j++);
  }
}

void initLED(void){
  P1SEL &=~ 0x1B
  P1DIR |=  0x1B
  P1 = 0;             
}

void initBUTTON(void){
  P1SEL &=~ 0x04
  P1DIR &=~ 0x04
  P1INP |=  0x04
  PICTL |=  0x02
}

#pragma vector = 0x7b;
__interrupt void P0_ISR(void)
