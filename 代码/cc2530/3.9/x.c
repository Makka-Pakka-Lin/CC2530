#include<iocc2530.h>
#define LED1 P1_3 
#define LED2 P1_4 
#define LED3 P1_0 
#define LED4 P1_1 
int flag = 0;
int a=0;
void delay(int x){
	int y;
	for(;x>0;x--)
for(y=325;y>0;y--);
}
void init(){
	P1SEL &= ~0x1f;
	P1DIR |=  0x1b;
        P1DIR &= ~0x04;
        
        LED1=0;
        LED2=0;
        LED3=0;
        LED4=0;
}
void ding(){
        CLKCONCMD &= ~0x7f;
        while(CLKCONSTA & 0x40);
        
        EA = 1;
        T1IE = 1;
        T1CTL =0x05;
}
void main(){
        ding();
        init();
        while(1){
          
        }
}

#pragma vector = T1_VECTOR
__interrupt void T1_ISR(void)
{ 
      if(flag<100) {
        flag++;     
     }
      else if(a==0){
        flag = 90;
        a=1;
        LED1 =1;
        delay(300);
        LED2 =1;
        delay(300);
        LED3 =1;
        delay(300);
        LED4 =1;
        delay(300);
        LED1 =0;
        LED2 =0;
        LED3 =0;
        LED4 =0;
        delay(300);
        LED4 =1;
        delay(300);
        LED3 =1;
        delay(300);
        LED2 =1;
        delay(300);
        LED1 =1;
      }
      else{
        a--;
        flag = 0;
        LED1 =0;
        LED2 =0;
        LED3 =0;
        LED4 =0;
      }
}