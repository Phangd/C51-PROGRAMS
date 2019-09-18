#include <reg52.h>
#define uchar unsigned char
#define uint  unsigned int

#define key P3
#define no_key 0xff
#define key_state0 0
#define key_state1 1
#define key_state2 2

code uchar num[]={0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};

uchar key_value;
bit flag;

void delay(uchar k){
	uchar i,j;
	for(i=k;i>0;i--)
		for(j=110;j>0;j--);
}

void scankey(){
	uchar key_state;
	uchar key_temp;
	uchar key1,key2;
	
	key = 0xf0;
	key1 = key;		//高四位为行标志,有键按下则相应行的I/O口变为0
	key1 = key&0xf0; 	//清掉低四位只保留高四位,值保存在key1中
	key = 0x0f;
	key2 = key;		//低四位为行标志,有键按下则相应行的I/O口变为0
	key2 = key&0x0f; 	//清掉高四位只保留低四位,值保存在key2中	
	key_temp = key1|key2;	//无键按下则为0xff,有键按下的会有类似(1101 1011)的值
				//				   key1 key2
	switch(key_state){
		case key_state0:
			if(key_temp != no_key){
				key_state = key_state1;
			}
		break;
		case key_state1:
			if(key_temp == no_key){
				key_state = key_state0;
			}
			else{
				switch(key_temp){
					case 0xee: key_value = 0;break;
					case 0xed: key_value = 1;break;
					case 0xeb: key_value = 2;break;
					case 0xe7: key_value = 3;break;
					case 0xde: key_value = 4;break;
					case 0xdd: key_value = 5;break;
					case 0xdb: key_value = 6;break;
					case 0xd7: key_value = 7;break;
					case 0xbe: key_value = 8;break;
					case 0xbd: key_value = 9;break;
					case 0xbb: key_value = 10;break;
					case 0xb7: key_value = 11;break;
					case 0x7e: key_value = 12;break;
					case 0x7d: key_value = 13;break;
					case 0x7b: key_value = 14;break;
					case 0x77: key_value = 15;break;
				}
			}
			key_state = key_state2;
		break;
		case key_state2:
			if(key_temp == no_key){
				key_state = key_state0;
			}
		break;
	}
	
}

void init_timer(){
	EA   = 0;
	TR0  = 0;
	TMOD = 0x01;
	TH0  = 0XDC;
	TL0  = 0X00;
	
	ET0  = 1;
	EA   = 1;
	TR0  = 1;
}

void timer0() interrupt 1 using 0{		
	TH0  = 0XDC;						//10ms
	TL0  = 0X00;
	flag = 1;
}

void main(){
	init_timer();
	while(1){
		if(flag == 1){
			flag = 0;
			scankey();
			P2 = num[key_value];
		}
	}
}
