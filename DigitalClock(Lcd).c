#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

#define key P3
#define no_key 0xff
#define key_state0 0
#define key_state1 1
#define key_state2 2

sbit lcden = P2^7;
sbit lcdrs = P2^6;
sbit lcdrw = P3^6;

sbit key1  = P3^2;
sbit key2  = P3^3;
sbit key3  = P3^4;
sbit key4  = P3^5;

void lcd_init();
void wcom();
void wdat();
void display();
void timeshow();
void dateshow();
void weekshow();
void keyscan();
void day_jud();
void key_disposal();

uint8 time_1s,time_250ms;
uint8 second,minute = 59,hour = 23;
uint8 day = 1,month = 1,week = 1;
uint16 year = 2019;
uint8 cursor;
uint8 key_value;
bit f_twinkle,f_mode,flag,f_pressing;

void delay(uint16 k){
	uint16 i,j;
	for(i=k;i>0;i--)
		for(j=110;j>0;j--);
}

void timer_init(){
	EA = 0;
	TR0 = 0;
	TR1 = 0;
	TMOD = 0x11;
	TH0 = 0x4C;		//50ms
	TL0 = 0x00;
	TH1 = 0x4C;
	TL1 = 0x00;
	
	ET0 = 1;		//允许Timer0中断
	ET1 = 1;
	EA  = 1;
	TR0 = 1;
	TR1 = 1;
}

void isr_timer0 () interrupt 1 using 0 {
	TH0 = 0x4C;		//50ms
	TL0 = 0x00;
	keyscan();
	if(++time_250ms >= 8){
		time_250ms = 0;
		f_twinkle = ~f_twinkle;
	}
}

void isr_timer1 () interrupt 3 using 1 {
	TH1 = 0x4C;
	TL1 = 0x00;
	if(++time_1s >= 20){
		time_1s = 0;
		second++;
	}
}

void wcom(uint8 com){
	lcdrw = 0;
	lcdrs = 0;
	P0 = com;
	lcden = 1;
	delay(5);
	lcden = 0;
}

void wdat(uint8 dat){
	lcdrw = 0;
	lcdrs = 1;
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

void timeshow(uint8 position,uint8 time){
	uint8 i,j;
	i = time/10;				//shiwei
	j = time%10;				//gewei
	wcom(0x80+0x40+position);
	wdat(0x30+i);				//shiwei
	wdat(0x30+j);				//gewei
}

void dateshow(uint8 position,uint8 date1){
	uint8 i,j;
	i = date1/10;				//shiwei
	j = date1%10;				//gewei
	wcom(0x80+position);
	wdat(0x30+i);				//shiwei
	wdat(0x30+j);				//gewei
}

void yearshow(uint8 position,uint16 year1){
	uint8 i,j,m,n;
	i = year1/1000;				//qianwei
	j = year1/100%10;			//baiwei
	m = year1/10%10;
	n = year1%10;
	wcom(0x80+position);
	wdat(0x30+i);				//qianwei
	wdat(0x30+j);				//baiwei
	wdat(0x30+m);				//shiwei
	wdat(0x30+n);				//gewei
}

void weekshow(uint8 position,uint8 week1){
	wcom(0x80+position);
	switch(week1){
		case 1:wdat(0x4d);wdat(0x4f);wdat(0x4e);break;
		case 2:wdat(0x54);wdat(0x55);wdat(0x45);break;
		case 3:wdat(0x57);wdat(0x45);wdat(0x44);break;
		case 4:wdat(0x54);wdat(0x48);wdat(0x55);break;
		case 5:wdat(0x46);wdat(0x52);wdat(0x49);break;
		case 6:wdat(0x53);wdat(0x41);wdat(0x54);break;
		case 7:wdat(0x53);wdat(0x55);wdat(0x4e);break;
		default:break;
	}
}

void day_jud(){
	if(second > 59){
		second = 0;
		if(++minute > 59){
			minute = 0;
			if(++hour > 23){
				hour = 0;
				if(++week >= 8)	week = 1;
				day++;
			}
		}
	}
	
	if(minute > 59){
		minute = 0;
		if(++hour > 23){
			hour = 0;
			if(++week > 7)	week = 1;
			day++;
		}
	}
	
	if(hour > 23){
		hour = 0;
		if(++week > 7)	week = 1;
		day++;
	}
	
	if(day > 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)){
		day = 1;
		month++;
		if(month >= 12){
			month = 1;
			year++;
		}
	}
	else if(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)){
		day = 1;
		month++;
	}
	else if(day > 29 && month == 2 && ((year%4 == 0 && year%100 != 0) || year%400 == 0)){
		day = 1;
		month++;
	}
	else if(day > 28 && month == 2){
		day = 1;
		month++;
	}
}

void display(){
	if(!f_mode){
		wcom(0x86);
		wdat(0x2d);							//"-"
		wcom(0x89);
		wdat(0x2d);							//"-"
		wcom(0x80+0x40+0x06);
		wdat(0x3a);							//":"
		wcom(0x80+0x40+0x09);
		wdat(0x3a);							//":"

		yearshow(2,year);
		dateshow(7,month);
		dateshow(0x0a,day);
		weekshow(0x0d,week);
		
		timeshow(4,hour);
		timeshow(7,minute);
		timeshow(0x0a,second);
	}
	else{
		wcom(0x86);
		wdat(0x2d);						//"-"
		wcom(0x89);
		wdat(0x2d);						//"-"
		wcom(0x80+0x40+0x06);
		wdat(0x3a);						//":"
		wcom(0x80+0x40+0x09);
		wdat(0x3a);						//":"
		wcom(0x80+0x40);
		wdat(0x53);						//"S"
		wdat(0x45);						//"E"
		wdat(0x54);						//"T"
		wdat(0x3a);						//":"
			
		if(f_twinkle && !f_pressing){
			if(cursor == 0) {
				wcom(0x82);
				wdat(0x20);
				wdat(0x20);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 1) {
				wcom(0x87);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 2) {
				wcom(0x8a);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 3) {
				wcom(0x8d);
				wdat(0x20);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 4) {
				wcom(0x80+0x44);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 5) {
				wcom(0x80+0x47);
				wdat(0x20);
				wdat(0x20);
			}
			if(cursor == 6) {
				wcom(0x80+0x4a);
				wdat(0x20);
				wdat(0x20);
			}
		}
		else {
			yearshow(2,year);
			dateshow(7,month);
			dateshow(0x0a,day);
			weekshow(0x0d,week);
			timeshow(4,hour);
			timeshow(7,minute);
			timeshow(0x0a,second);
		}
	}
}

void keyscan(){
	static uint8 key_state;
	
	switch(key_state){
		case key_state0:
			if(!key1 || !key2 || !key3 || !key4){
				key_state = key_state1;
			}
		break;
		case key_state1:
			f_pressing = 1;                     //按下后置一，处理函数中不处理按键
			if(!key1){
				TR1 = ~TR1;
				f_mode = ~f_mode;
				key_value = 1;
				key_state = key_state2;
			}
			else if(!key2){
				key_value = 2;
				key_state = key_state2;
			}
			else if(!key3){
				key_value = 3;
				key_state = key_state2;
			}
			else if(!key4){
				key_value = 4;
				key_state = key_state2;
			}
			else{
				key_state = key_state0;
			}
		break;
		case key_state2:
			if(key1 && key2 && key3 && key4){
				f_pressing = 0;                //松手后清零，处理按键反馈
				key_state = key_state0;
			}
		break;
	}
}

void key_disposal(){
	if(!f_pressing){
		if(key_value == 1){
			key_value = 0;
			cursor = 0;
			lcd_init();
		}
		else if(key_value == 2){
			key_value = 0;
			cursor++;
			if(cursor > 6){
				cursor = 0;
			}
		}
		else if((key_value == 3) && f_mode){
			key_value = 0;
			if(cursor == 0){
				year++;
			}
			else if (cursor == 1){
				month++;
				if(month > 12){
					month = 1;
				}
			}
			else if(cursor == 2){
				if(++day > 31) day = 1;
			}
			else if(cursor == 3){
				if(++week > 7) week = 1;
			}
			else if(cursor == 4){
				if(++hour > 23) hour = 0;
			}
			else if(cursor == 5){
				if(++minute > 59) minute = 0;
			}
			else if(cursor == 6){
				if(++second > 59) second = 0;
			}
		}
		else if((key_value == 4) && f_mode){
			key_value = 0;
			if(cursor == 0 && year > 0){
				year--;
			}
			else if (cursor == 1 && month > 1){
				month--;
			}
			else if(cursor == 2 && day > 1){
				day--;
			}
			else if(cursor == 3 && week > 1){
				week--;
			}
			else if(cursor == 4 && hour > 0){
				hour--;
			}
			else if(cursor == 5 && minute > 0){
				minute--;
			}
			else if(cursor == 6 && second > 0){
				second--;
			}
		}
	}
}

void main(){
	timer_init();
	lcd_init();
	while(1){
		day_jud();
		key_disposal();
		display();
	}
}
