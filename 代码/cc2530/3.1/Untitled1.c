#include<iocc2530.h>
int flag = 0;
void delay(int x){
	int y;
	for(;x>0;x--)
for(y=325;y>0;y--);
}
void init(){
	P1SEL &= ~0x1b;
	P1DIR = 0x1b;
	P1 = 0;
	IEN2 = 0x10;
	P1IEN = 0x04;
	PICTL = 0x02;
	EA = 1;
	T3CTL = 0x14;
	T4CTL = 0x14;
	T3CCTL0 = 0x64;
	T3CCTL1 = 0x64;
	T4CCTL0 = 0x64;
	T4CCTL1 = 0x64;
}
void main(){
        int a=0x00;
	init();
	while(1){
	switch(flag){
	case 1:
	P1SEL = 0x1b;
        for (a=0x00;a<0xff;a++){
          T3CC0 = a;delay(50);
          T3CC1 = a;delay(50);
          T4CC0 = a;dealy(50);
          T4CC1 = a;delay(50);
        }
        for(a=0xff;a>0x00;a--){
          T3CC0 = a;delay(50);
          T3CC1 = a;delay(50);
          T4CC0 = a;dealy(50);
          T4CC1 = a;delay(50);
        }
        case 2: 
	P1SEL = ~0x1b;
	break;
}
}
}
#pragma vector = 0x7b
__interrupt void zd(){
	if(!P1_2){
	delay(100);
	while(P1_2);
	flag++;
	if(flag ==5)
	flag = 1;
	P1IFG = 0;
}
P1IF = 0;
}  