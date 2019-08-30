#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int

code unsigned char num[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
uchar n0=0,n1=1,n2=2,n3=3,n4=4,n5=5,n6=6;
uchar flag;
void delay(int x){
	int i,j;
	for(i=x;i>0;i--)
		for(j=110;j>0;j--);
}

void init_timer(){
	EA   = 0;
	TR0  = 0;
	TMOD = 0x01;
	TH0  = 0xEE;		//5ms
	TL0  = 0x00;
	
	EA   = 1;
	ET0	 = 1;
	TR0  = 1;
}

void timer0_serive() interrupt 1 using 0{
	TH0  = 0xEE;
	TL0  = 0x00;
	flag++;
	if(flag == 100){
		flag = 0;
		n0++;
		if(n0>9) n0=0;
		n1++;
		if(n1>9) n1=0;
		n2++;
		if(n2>9) n2=0;
		n3++;
		if(n3>9) n3=0;
		n4++;
		if(n4>9) n4=0;
		n5++;
		if(n5>9) n5=0;
		n6++;
		if(n6>9) n6=0;
	}
}

void digshow(){
	//uchar i,j;
	P2 = 0x70;
	P0 = num[n0];
	delay(5);
	
	P2 = 0x71;
	P0 = num[n1];
	delay(5);
	
	P2 = 0x72;
	P0 = num[n2];
	delay(5);
	
	P2 = 0x73;
	P0 = num[n3];
	delay(5);
	
	P2 = 0x74;
	P0 = num[n4];
	delay(5);
	
	P2 = 0x75;
	P0 = num[n5];
	delay(5);
	
	P2 = 0x76;
	P0 = num[n6];
	delay(5);
}

void main(){
	init_timer();
	while(1){
		digshow();
	}
}
