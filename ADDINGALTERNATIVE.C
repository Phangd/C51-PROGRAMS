#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int

uchar flag,seconds,m,n;
code unsigned char num[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
code unsigned char seg[]={0xFE,0xFD,0xFB,0xF7};
void delay(uchar k){
	uchar i,j;
	for(i=k;i>0;i--)
		for(j=110;j>0;j--);
}

void init_timer(){
	EA   = 0;
	TR0  = 0;
	ET0  = 0;
	TMOD = 0x01;
	TH0  = 0xEE;
	TL0  = 0x00;
	
	EA   = 1;
	ET0  = 1;
	TR0  = 1;
}

void timer0()interrupt 1 using 0{
	TH0  = 0xEE;
	TL0  = 0x00;
	
	flag++;
	if(flag >= 100){
		flag = 0;
		m++;			//位数和显示的数字要同时增加
		n++;			//实现按位显示数字递增
	}
}

void display(){
	if(m > 9) m = 0;
	if(n > 3) n = 0;
	
	P2 = 0xff;
	P0 = num[m];
	P2 = seg[n];
	delay(5);
}

void main(){
	init_timer();
	while(1){
		display();
	}
}
