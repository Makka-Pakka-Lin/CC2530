#include<ioCC2530.h>
#define LED1 P1_3
#define KEY1 P1_2
int flag = 0;
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
void delay(int x,int m){
	for(int y=x; y >0;y--){
		for(int z = 573; z>0;z--){
		if(m!=flag){
		return;
		}
		}
	}
}
void main(){
	init();
        while(1){
	if(flag == P1_2)
          LED1 = 1;
        else
          LED1 = 0;
        }
}
#pragma vector = 0x7b 
__interrupt void zd(void)
{
	if(!P1_2){
            while(!P1_2);
          flag++;
	if(flag == 3 )
	flag = 0;	
}
	P1IFG = 0;
P1IF = 0;
}