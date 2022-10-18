#include<iocc2530.h>
#define LED1 P1_0
#define LED2 P1_1
#define SW1  P1_2
#define LED3 P1_3
#define LED4 P1_4
int flag =0;
int found =400;
int a=0;
int b=0;
int c=0;
void init(){
	P1SEL &= ~0x1F;
	P1DIR |= ~0x04;
	P1DIR &= 0x1B;
        P1 = 0;
        LED1=1;
        LED2=0;
        
        IEN2 = 0x10;
        P1IEN = 0x04;
        PICTL = 0x02;
        EA = 1;
        
}
void ding(){
        CLKCONCMD &= ~0x7f;
        while(CLKCONSTA & 0x40);

        EA = 1;
        T1IE = 1;
        T1CTL =0x05;
}

void main(){
  int c=0;
  ding();
  init();
  while(1){
   if(flag==1)
   {found=0;flag+=1;}
   if(flag==2){
            if(c==1){LED1=0;found=240;}
            if(c==2){found=240;LED2=1;}
            if(c==3){found=240;LED1=1;}
            if(c==4){found=240;LED1=0;}
            if(c==5){found=240;LED2=1;}
            if(c==6){found=240;LED2=0;c=3;}
   }
   if(flag==3){
            LED1=1;
            LED2=1;
            LED4=0;
          }
  }
  
}
#pragma vector = T1_VECTOR
__interrupt void T1_ISR(void)
{ 
  if(found<300){
    found++; 
  }
  if(found==300)
  {c+=1;found+=10;LED4=1;}
}

#pragma vector = 0x7b 
__interrupt void zd(void)
{
	if(!P1_2){
          while(!P1_2);
          flag++;
          if(flag==4){flag=1;c=1;LED1=1;LED2=0;}
          }
        
P1IFG = 0;
P1IF = 0;
}

