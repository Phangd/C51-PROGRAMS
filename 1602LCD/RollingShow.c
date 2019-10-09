#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

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

void wchar(uint8 *char1){				//写字符串
	while(*char1){
		wdat(*char1);
		char1++;
	}
}

void lcd_init(){
	wcom(0x38);
	wcom(0x01);
	wcom(0x06);
	wcom(0x0c);
}

void main(){
	uint8 i=0,j=0; 
	lcd_init();
	delay(5);
	wcom(0x80);
	wchar("--Hello World!--");
	while(1){
		wcom(0x80+0x40);
		for(i=j;i<40+j;i++){			//将指针开头不断累加，但循环次数始终为40次
			wdat(char2[i]);				//指针累加后下一次显示的内容则为前一次的内容往前移了一位
		}
		j++;
		if(j>9){						//多出的字数显示完毕后，将指针开头置回第0位
			j=0;
			delay(500);					//显示完所有内容后停留一秒钟
		}
		delay(500);						//移位间隔
	}
}
