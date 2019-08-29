#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int

sbit redew 	  = P0^0;
sbit yellowew = P0^1;
sbit greenew  = P0^2;
sbit redns 	  = P0^3;
sbit yellowns = P0^4;
sbit greenns  = P0^5;
sbit nsduan	  = P3^0;
sbit nswei    = P3^1;

code unsigned char num[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
code unsigned char seg[]={0xfa,0xf5,0xf4,0xf8};
uchar flag;
uchar seconds   = 25;

void delay(uchar x){
	uchar i,j;
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
	if(flag == 200){
		seconds--;
		flag = 0;
		if(seconds <= 0) seconds=25;
	}
}

void timeshow(){
	uchar redshi,redge,yelloshi,yelloge,greenshi,greenge;
	redshi 	 = (seconds-15)/10;
	redge  	 = (seconds-15)%10;
	yelloshi = (seconds-11)/10;
	yelloge  = (seconds-11)%10;
	greenshi = seconds/10;
	greenge  = seconds%10;
	if(seconds <= 25 && seconds >14){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[redshi];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[redge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
	}
	
	if(seconds <= 14 && seconds >10){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[yelloshi];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[yelloge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
	}
	
	if(seconds <= 10 && seconds >=0){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[greenshi];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0xf0;
		
		P2    = num[greenge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0xf0;
		delay(5);
	}
}

void ledshow(){
	// if(redtime > 0) redns = 0; 
}

void main(){
	init_timer();
	while(1){
		timeshow();
	}
}
