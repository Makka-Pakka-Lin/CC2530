#include<iocc2530.h>
char data[]="Ð¡°¢Î°°®µç¶¯";

void delay(int i){
  int j,k;
  for(k=0;k<i;k++){
    for(j=0;j<500;j++);
  }
}

void init(){
  CLKCONCMD &=~0x40;
  while(CLKCONSTA & 0x40);
  CLKCONCMD &=~0x47;
  PERCFG = 0x00;
  P0SEL|=0x3C;
  
  P2DIR &=~0xc0;
  U0CSR = 0x80;
  U0GCR = 9;
  U0BAUD = 59;
  UTX0IF = 0;
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
  while(1){
    uart(data,sizeof(data));
    delay(1000);
  }
}
