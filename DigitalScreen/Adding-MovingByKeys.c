#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

code uint8 num[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};
code uint8 seg[]={0xFE,0xFD,0xFB,0xF7};

sbit segadd = P3^2;
sbit segsub = P3^3;
sbit numadd = P3^4;
sbit numsub = P3^5;

uint8 position,number;

void delay(uint8 k){
	uchar i,j;
	for(i=k;i>0;i--)
		for(j=110;j>0;j--);
}

void display(){
	P2 = 0xff;
	P0 = num[number];
	P2 = seg[position];
	delay(5);
}

void scankey(){
	uint8 temp,judtime,keyflag,oldkeyvalue;
	temp = P3&0xff;
	if(temp!=0xff){
		judtime++;
		if(judtime >= 10){
			oldkeyvalue = temp;
			keyflag = 1;
		}
	}
	else{						//松手后再处理
		judtime = 0;				//不管是否按下都清掉按键持续时间
		if(keyflag == 1){
			keyflag = 0;
			switch(oldkeyvalue){
				case 0xfb:	
						position++;
						if(position > 3) position = 0;
				break;
				case 0xf7:	
						position--;
						if(position > 3) position = 3;
				break;
				case 0xef:
						number++;
						if(number > 9) number = 9;
				break;
				case 0xdf:
						number--;
						if(number > 9) number = 0;
				break;
			}
		}
	}
}
	
void main(){
	while(1){
		scankey();
		display();
	}
}
