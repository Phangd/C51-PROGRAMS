#include "reg52.h"
#define uchar unsigned char
#define uint  unsigned int

sbit stop = P3^2;
sbit reset = P3^3;

int second=5;
int microsecond=100;
int flag= 1000;
int num,micronum;
uchar j = 0;

code unsigned char digsel[4]={0xFE,0xFD,0xFB,0xF7};
code unsigned char digdis[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
code unsigned char digdis_dot[]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

void delay(uchar k);
void dig_open(void);
void timer0_define(void);
void timer0_main(void);

void dig_show(void){
	delay(10);
	P2 = 0xFF;
	P0 = digdis[second/10];
	P2 = digsel[0];
	delay(10);
	P2 = 0xFF;
	P0 = digdis[second%10];
	P2 = digsel[1];
	delay(10);
	P2 = 0xFF;
	P0 = digdis[microsecond/10];
	P2 = digsel[2];
	delay(10);
	P2 = 0xFF;
	P0 = digdis[microsecond%10];
	P2 = digsel[3];
}

void delay(uchar k){
	unsigned char i,j;
	for(i=k;i>0;i--){
		for(j=110;j>0;j--);
	}
}

void timer0_define(void){
	EA=0;
	TR0=0;
	TMOD=0x01;
	TL0=0x00;
	TH0=0xEE;
	
	PT0=1;
	ET0=1;
	TR0=1;
	EA=1;
}

void timer0_main(void) interrupt 1 {
	TL0=0x00;
	TH0=0xEE;
	
	num++;
	micronum++;
	
	if(micronum%2==0&&second!=0){
		microsecond--;
		if(microsecond==0) {
			second--;
			microsecond=100;
		}
		if(second == 0) second=0;
	}
	
	if(micronum>=flag&&micronum<(flag+100)){
		if(microsecond==0) microsecond=100;
		microsecond--;
	}

	if(num==200){
		num=0;
		P1=(P1>>1)|0x80;
		if(P1==0xFF) P1=0x7F;
	}
}

void main(void){
	P1 = 0x7F ;
	timer0_define();
	while(1){
		dig_show();
		if(stop == 0){
			delay(20);
			if(stop == 0){
				TR0=!TR0;
				while(!stop);
			}
		}
		if(reset == 0){
			delay(20);
			if(reset == 0){
				second=5;
				microsecond=100;
				micronum=0;
				while(!reset);
			}
		}
	}
}
