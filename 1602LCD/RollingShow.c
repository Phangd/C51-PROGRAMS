#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

code uint8 char1[]="--Hello World!-- --Hello World!--";
code uint8 char2[]="Https://github.com/Phangd";
sbit lcdrw = P3^6;
sbit lcdrs = P2^6;
sbit lcden = P2^7;

void delay(uint16 k){
	uint16 i,j;
	for(i=k;i>0;i--)
		for(j=110;j>0;j--);
}

void wcom(uint8 com){
	lcdrs = 0;
	lcdrw = 0;
	P0 = com;
	lcden = 1;
	delay(5);
	lcden = 0;
}

void wdat(uint8 dat){
	lcdrs = 1;
	lcdrw = 0;
	P0 = dat;
	lcden = 1;
	delay(5);
	lcden = 0;
}

void lcd_init(){
	wcom(0x38);
	wcom(0x01);
	wcom(0x06);
	wcom(0x0c);
}

void main(){
	uint8 num;
	lcd_init();
	delay(5);
	wcom(0x80);
	while(1){
		while(char1[num] != '\0'){
			wdat(char1[num]);
			num++;
		}
		num = 0;
		while(char2[num] != '\0'){
			wcom(0x80+0x40+num);
			wdat(char2[num]);
			num++;
			delay(200);
			if(num >= 15){
				wcom(0x18);
				delay(300);
			}
		}
		if(num >= 25){
			delay(1000);
			wcom(0x01);
			num = 0;
		}
	}
}
