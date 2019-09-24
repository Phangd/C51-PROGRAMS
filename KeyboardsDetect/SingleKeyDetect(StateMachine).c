#include <reg52.h>
#define uchar unsigned char
#define	uint  unsigned int

#define no_key 0xff
#define key_state0 0
#define key_state1 1
#define key_state2 2

sbit key_s1 = P3^2;
sbit key_s2 = P3^3;
sbit key_s3 = P3^4;

uchar seconds,flag;

code uchar num[]={0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
code uchar seg[]={0xFE,0xFD,0xFB,0xF7};

void delay(uchar k){
	unsigned char i,j;
	for(i=k;i>0;i--){
		for(j=110;j>0;j--);
	}
}

void timer_init(){
	EA   = 0;
	TR0  = 0;
	TMOD = 0x01;
	TH0  = 0xDC;
	TL0  = 0x00;
	
	ET0  = 1;
	EA   = 1;
	TR0  = 0;
}

void timer0() interrupt 1 using 0{
	TH0   = 0xDC;
	TL0   = 0x00;

	flag++;
	if(flag >= 100){
		flag = 0;
		seconds++;
		if(seconds >= 100) seconds = 0;
	}
}

void key_handle(){
	static char key_state;				//此处一定要用static定义,或定义到全局变量中
	uchar key_press;
	uchar key_press1;
	uchar key_press2;
	
	key_press  = key_s1;
	key_press1 = key_s2;
	key_press2 = key_s3;
	switch(key_state){
		case key_state0:
		if(!key_press||!key_press1||!key_press2) key_state = key_state1;
		break;
		
		case key_state1:
		if(!key_press) {
			TR0 = 1;
			key_state = key_state2;
		}
		else if(!key_press1) {
			TR0 = 0;
			key_state = key_state2;
		}
		else if(!key_press2) {
			if(TR0 == 0) seconds = 0;
			key_state = key_state2;
		}
		else{
			key_state = key_state0;
		}
		break;
		
		case key_state2:
		if(key_press||key_press1||!key_press2) {
			key_state = key_state0;
		}
		break;
	}
}

void display(){
	P2 = 0xff;
	P0 = num[seconds/10];
	P2 = seg[0];
	delay(10);
	
	P2 = 0xff;
	P0 = num[seconds%10];
	P2 = seg[1];
	delay(10);
}

void main(){
	timer_init();
	while(1){	
		key_handle();
		display();
	}
}
