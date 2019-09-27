#include <reg52.h>
#include <intrins.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

sbit power_onoff = P3^0;
code uint8 charcode[4][8]={
	{0x10,0x28,0x44,0x82,0x10,0x28,0x44,0x82},
	{0x28,0x44,0x82,0x10,0x28,0x44,0x82,0x10},
	{0x44,0x82,0x10,0x28,0x44,0x82,0x10,0x28},
	{0x82,0x10,0x28,0x44,0x82,0x10,0x28,0x44}
	};

uint8 i,j,flag;

void Key_down()interrupt 0{			
	P0 = 0xff;
	P1 = 0x80;
	TR0 = 1;						
}

void timer0()interrupt 1{
	TH0 = (65536-2000)/256;
	TL0 = (65536-2000)%256;
	flag++;
	if(flag >= 100){
		flag = 0;
		i++;
	}
	if(i > 3) i = 0;
	P1 = _crol_(P1,1);
	P0 = ~charcode[i][j];
	j = (j + 1) % 8;
}

void main(){
	TMOD = 0x01;
	TH0 = (65536-2000)/256;
	TL0 = (65536-2000)%256;
	IT0 = 1;
	IE  = 0x83;
	i = 0;
	j = 0;
	P0 = 0xff;
	P1 = 0x80;
	while(1);
}
