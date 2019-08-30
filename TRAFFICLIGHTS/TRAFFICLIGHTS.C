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
code unsigned char seg[]={0xaa,0x55,0x44,0x88};
uchar flag;
uchar seconds   = 30;

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
		if(seconds == 0) seconds = 30;		//uchar类型没有负数,0-1后为255
	}
}

void timeshow(){
	uchar redshi,redge,yelloshi,yelloge,greenshi,greenge,yelloshi1,yelloge1;
	redshi 	  = (seconds-20)/10;
	redge  	  = (seconds-20)%10;
	yelloshi  = (seconds-16)/10;
	yelloge   = (seconds-16)%10;
	greenshi  = (seconds-5)/10;
	greenge   = (seconds-5)%10;
	yelloshi1 = (seconds-1)/10;
	yelloge1  = (seconds-1)%10;
	
	if(seconds <= 30 && seconds >19){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[redshi];		//南北红灯
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[redge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
	}
	
	if(seconds <= 19 && seconds >15){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[yelloshi];		//全黄
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[yelloge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
	}
	
	if(seconds <= 15 && seconds >=5){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[greenshi];		//南北绿灯
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[greenge];
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
	}
	
	if(seconds < 5 && seconds >=0){
		P2    = seg[1];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[yelloshi1];		//全黄
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
		
		P2    = seg[0];
		nswei = 1;
		nswei = 0;
		P2    = 0x00;
		
		P2    = num[yelloge1];
		nsduan= 1;
		nsduan= 0;
		P2    = 0x00;
		delay(5);
	}
}

void ledshow(){
	if(seconds <= 30 && seconds > 23){
		P0 	  = 0xDE;				//绿红灯常亮
	}
	switch(seconds){				//最后4秒交通灯闪烁
		case 23:
			P0  = 0xFF;
			break;
		case 22:
			P0	= 0xDE;
			break;
		case 21:
			P0	= 0xFF;
			break;
		case 20:
			P0	= 0xDE;
			break;
	}
	
	if(seconds <= 19 && seconds > 15){
		P0 	  = 0xED;				//黄灯常亮
	}
	
	if(seconds <= 15 && seconds > 8){
		P0 	  = 0xF3;
	}
	switch(seconds){				//最后4秒交通灯闪烁
		case 8:
			P0	= 0xFF;
			break;
		case 7:
			P0	= 0xF3;
			break;
		case 6:
			P0	= 0xFF;
			break;
		case 5:
			P0	= 0xF3;
			break;
	}
	
	if(seconds < 5 && seconds >= 0){
		P0 	  = 0xED;				//黄灯常亮
	}
}

void main(){
	init_timer();
	while(1){
		timeshow();
		ledshow();
	}
}
