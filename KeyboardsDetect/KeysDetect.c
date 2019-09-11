#include <reg52.h>

int number;
unsigned char num_key=0;
code unsigned char num[]={0xc0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xf8,0x80,0x90};

sbit key1 = P3^2;
sbit key2 = P3^3;

// void delay(unsigned char x){
	// unsigned char i,j;
	// for(i=0;i<x;i++)
		// for(j=110;j>0;j--);
// }

// void init_timer(){
	// EA   = 0;
	// TR0  = 0;
	// TMOD = 0x01;
	// TH0  = 0XEE;
	// TL0  = 0X00;
	
	// ET0  = 1;
	// EA   = 1;
	// TR0  = 1;
// }

// void timer0() interrupt 1 using 0{
	// if(key1 == 0){
		// delay(2);
		// if(key1 == 0){
			// number++;
			// if(number > 9999) number = 9999;
			// while(!key1)digitalshow();
		// }
	// }
	// if(key2 == 0){
		// delay(2);
		// if(key2 == 0){
			// number--;
			// if(number < 0) number = 0;
			// while(!key2)digitalshow();
		// }
	// }
// }

void digitalshow(){
	unsigned char qian,bai,shi,ge;
	qian = number/1000;
	bai  = number/100%10;
	shi  = number/10%10;
	ge   = number%10;
	
	P2 = 0xff;
	P0 = num[qian];
	P2 = 0xfe;
	delay(5);
	P2 = 0xff;
	P0 = num[bai];
	P2 = 0xfd;
	delay(5);
	P2 = 0xff;
	P0 = num[shi];
	P2 = 0xfb;
	delay(5);
	P2 = 0xff;
	P0 = num[ge];
	P2 = 0xf7;
	delay(5);
}

void scankey(){
	unsigned char temp = 0;
	static unsigned char temp_code=0;
	static unsigned char num_check=0;
	static unsigned char key_flag=0;
	temp=P3&0xFF;			//P3		:x x key4 key3 key2 key1 x x
	if(temp!=0xFF){			//P3原始状态:1 1 1 1 1 1 1 1
		num_check++;		//有键按下则:    0 0 0 0    TEMP任意一位变0
		if(num_check==10){
			key_flag=1;
			temp_code=temp;
		}
	}
	else{				//松手后再处理
		num_check=0;		//不管是否按下都清掉按键持续时间
		if(key_flag==1){
			key_flag=0;
			switch(temp_code){
				case 0xfb: 
					number++;
					if(number > 9999) number = 9999;
				break;
				case 0xf7:
					number--;
					if(number < 0) number = 0;
				break;
				
				//case 0xfb: num_key=1;
				//break;
				//case 0xf7: num_key=2;	
				//break;
				//case 0xef: num_key=3;
				//break;
				//case 0xdf: num_key=4;
				//break;
			}
		}
	}
}


void main(){
	//init_timer();	
	P3 = 0xFF;
	while(1){
		scankey();
		digitalshow();
	}
}
