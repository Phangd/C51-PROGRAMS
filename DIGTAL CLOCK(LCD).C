#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char

sbit key1 = P3^2 ;		//键盘

sbit lcden = P2^7;		//LCD使能
sbit lcdrs = P2^6;		//LCD数据/命令选择引脚
sbit lcdrw = P3^6;		//LCD读写命令选择
sbit key_mode = P3^2;	//切换时间/调整模式
sbit key_min = P3^3;
sbit key_add = P3^4;
sbit key_sub = P3^5;

uint second = 51;
uint minute = 59;
uint hour 	= 23;
uint year 	= 19;
uint month 	= 6;
uint day 	= 20;
uchar flag  = 0;
uchar mode  = 0;

void delay();
void delayhalf();
void shortdelay();
void lcd_wcom();
void lcd_init();
void lcd_wdat();

void init_int();
void init_ser();
void waibu_ser();

void keyscan();
void timeshow();
void lcdinitshow();

void delay(){						//延时1s
	unsigned char i,j,k;
	for(i=26;i>0;i--){
		for(j=202;j>0;j--){
			for(k=81;k>0;k--){;}
		}
	}
}

void delayhalf(){					//延时0.5s
	unsigned char i,j,k;
	for(i=13;i>0;i--){
		for(j=202;j>0;j--){
			for(k=81;k>0;k--){;}
		}
	}
}

void shortdelay(uint n){			//短延时
	uint i,j;
	for(i=n;i>0;i--){
		for(j=110;j>0;j--){;}
	}
}

void lcd_wcom(uchar com){		
	lcdrs = 0;						//写命令模式
	lcdrw = 0;						//写入到LCD屏幕
	P0 = com;
	shortdelay(5);
	lcden = 1;
	shortdelay(5);
	lcden = 0;
}

void lcd_wdat(uchar dat){		
	lcdrs = 1;						//写数据模式
	lcdrw = 0;						//写入到LCD屏幕
	P0 = dat;
	shortdelay(5);
	lcden = 1;
	shortdelay(5);
	lcden = 0;
}

void lcd_init(){					//初始化LCD
	lcd_wcom(0x01);					//清屏
	lcd_wcom(0x38);					//四位数据，两行显示，5*7
	lcd_wcom(0x0C);					//无光标，打开显示
	lcd_wcom(0x06);					//光标右移，屏幕不移动
	lcd_wcom(0x80);					//设置数据指针起点
}

void timeshow(uchar add,uchar time){
	uchar i,j;
	i = time/10;					//取时间的十位
	j = time%10;					//取时间的个位
	lcd_wcom(0x80+add);				//第1排第add个位置
	lcd_wdat(0x30+i);				//显示数字，0x30是0，0x31是1
	lcd_wdat(0x30+j);				//显示数字，0x32是2，0x33是3...
}

void dateshow(uchar add,uchar date){
	uchar x,y;
	x = date/10;
	y = date%10;
	lcd_wcom(0xC0+add);				//第2排第add个位置
	lcd_wdat(0x30+x);				//显示数字，0x30是0，0x31是1
	lcd_wdat(0x30+y);				//显示数字，0x32是2，0x33是3...
}


void init_int(void){				//初始化定时器中断0和外部中断0
	EA 	 = 0;
	TR0  = 0;
	TMOD = 0x01;
	
	
	EA 	= 1;
	PT0 = 1;
	ET0 = 1;						//允许定时器中断 优先级第3
	// EX0 = 1;						//允许外部中断0中断，优先级第1
	// IT1 = 1;
	TR0 = 1;
}

void init_ser(void) interrupt 1		//定时器0中断服务函数
{	
	TL0  = 0x00;
	TH0  = 0xEE;					//5ms溢出一次
	flag++;
	if(flag == 200){
		flag = 0;
		second++;
		if(second == 60){
			second = 0;
			minute++;
			if(minute == 60){
				minute = 0;
				hour++;
				if(hour == 24){
					hour = 0;
					day++;
					if(day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)){
						day = 1;
						month++;
						if(month > 12){
							month = 1;
							year++;
						}
					}
					if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
						day = 1;
						month++;
					}
					if(day == 29 && month ==2 && (year%4 != 0)){
						day = 1;
						month++;
					}
					if(day == 29 && month ==2 && year%100 == 0 && year%400 != 0){
						day = 1;
						month++;
					}
					if(day == 30 && month ==2 && ((year%100 == 0 && year%400 == 0)|| (year%100 != 0 && year%4 == 0))){
						day = 1;
						month++;
					}
				}
			} 
		}
	}
}

void keyscan(void){
	uchar i,j,k,m;
	if(key_mode == 0){
		shortdelay(50);
		if(key_mode == 0){
			i = 0;
			j = 0;
			k = 0;
			m = 0;
			mode++;
			while(!key_mode);
		}
		if(mode > 1) mode = 0;
	}
	
	if(mode == 0){
		TR0 = 1;
		lcd_wcom(0x0C);
		timeshow(6,hour);
		timeshow(9,minute);
		timeshow(0x0C,second);
		dateshow(6,year);
		dateshow(9,month);
		dateshow(0x0C,day);
	}
	
	if(mode == 1){					//调节时间
		TR0  = 0;					//暂停计数
		if(key_min == 0){
			shortdelay(50);
			if(key_min == 0){		//去抖
				i+= 3;				//光标位置
				k++;				//换行变量
				if(k >= 3 && k < 6) {
					i = 0;
					i = i + 3*m; 
					j = 0x40;
					m++;
				}
				if(k >= 6){
					i = 0;
					j = 0;
					k = 0;
					m = 0;
				}
				while(!key_min) {;}
			}
		}

		lcd_wcom(0x87+i+j);			//光标闪烁
		lcd_wcom(0x0E);
		shortdelay(100);
		lcd_wcom(0x87+i+j);			//光标闪烁
		lcd_wcom(0x0C);
		shortdelay(150);


		timeshow(6,hour);
		timeshow(9,minute);
		timeshow(0x0C,second);
		dateshow(6,year);
		dateshow(9,month);
		dateshow(0x0C,day);
		
		if(key_add == 0){
			shortdelay(50);
			if(key_add == 0){
				if(k == 0){
					hour++;
					if(hour == 24){
						hour = 0;
						day++;
						if(day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)){
							day = 1;
							month++;
							if(month > 12){
								month = 1;
								year++;
							}
						}
						if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
							day = 1;
							month++;
						}
						if(day == 29 && month ==2 && (year%4 != 0)){
							day = 1;
							month++;
						}
						if(day == 29 && month ==2 && year%100 == 0 && year%400 != 0){
							day = 1;
							month++;
						}
						if(day == 30 && month ==2 && ((year%100 == 0 && year%400 == 0)|| (year%100 != 0 && year%4 == 0))){
							day = 1;
							month++;
						}
					}
				}
				
				if(k == 1){
					minute++;
					if(minute == 60){
						minute = 0;
						hour++;
						if(hour == 24){
							hour = 0;
							day++;
							if(day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)){
								day = 1;
								month++;
								if(month > 12){
									month = 1;
									year++;
								}
							}
							if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
								day = 1;
								month++;
							}
							if(day == 29 && month ==2 && (year%4 != 0)){
								day = 1;
								month++;
							}
							if(day == 29 && month ==2 && year%100 == 0 && year%400 != 0){
								day = 1;
								month++;
							}
							if(day == 30 && month ==2 && ((year%100 == 0 && year%400 == 0)|| (year%100 != 0 && year%4 == 0))){
								day = 1;
								month++;
							}
						}
					}
				}
				
				if(k == 2){
					second++;
					if(second == 60){
						second = 0;
						minute++;
						if(minute == 60){
							minute = 0;
							hour++;
							if(hour == 24){
								hour = 0;
								day++;
								if(day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)){
									day = 1;
									month++;
									if(month > 12){
										month = 1;
										year++;
									}
								}
								if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
									day = 1;
									month++;
								}
								if(day == 29 && month ==2 && (year%4 != 0)){
									day = 1;
									month++;
								}
								if(day == 29 && month ==2 && year%100 == 0 && year%400 != 0){
									day = 1;
									month++;
								}
								if(day == 30 && month ==2 && ((year%100 == 0 && year%400 == 0)|| (year%100 != 0 && year%4 == 0))){
									day = 1;
									month++;
								}
							}
						}
					}
				}
				
				if(k == 3){
					year++;
				}
				
				if(k == 4){
					month++;
					if(month > 12) {
						year++;
						month = 1;
					}
				}
				
				if(k == 5){
					day++;
					if(day == 32 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)){
						day = 1;
						month++;
						if(month > 12){
							month = 1;
							year++;
						}
					}
					if(day == 31 && (month == 4 || month == 6 || month == 9 || month == 11)){
						day = 1;
						month++;
					}
					if(day == 29 && month ==2 && (year%4 != 0)){
						day = 1;
						month++;
					}
					if(day == 29 && month ==2 && year%100 == 0 && year%400 != 0){
						day = 1;
						month++;
					}
					if(day == 30 && month ==2 && ((year%100 == 0 && year%400 == 0)|| (year%100 != 0 && year%4 == 0))){
						day = 1;
						month++;
					}
				}
				while(!key_add);
			}
		}
		
		if(key_sub == 0){
			shortdelay(50);
			if(key_sub == 0){
				if(k == 0 && hour > 0){
					hour--;
				}	
				if(k == 1 && minute > 0){
					minute--;
				}
				if(k == 2 && second > 0){
					second--;
				}
				if(k == 3 && year > 0){
					year--;
				}
				if(k == 4 && month > 1){
					month--;
				}
				if(k == 5 && day > 1){
					day--;
				}
				while(!key_sub);
			}
		}
	}
}

void lcdinitshow(void){
	lcd_init();
	
	lcd_wcom(0x81);				//T
	lcd_wdat(0x54);
	lcd_wcom(0x82);				//I
	lcd_wdat(0x49);
	lcd_wcom(0x83);				//M
	lcd_wdat(0x4D);
	lcd_wcom(0x84);				//E
	lcd_wdat(0x45);
	
	lcd_wcom(0x85);				//显示冒号 在第5位
	lcd_wdat(0x3A);				//显示冒号
	
	lcd_wcom(0x88);				//显示冒号	在第8位
	lcd_wdat(0x3A);				//显示冒号
	
	lcd_wcom(0x8B);				//显示冒号	在第11位
	lcd_wdat(0x3A);				//显示冒号
	
	lcd_wcom(0xC1);				//D
	lcd_wdat(0x44);
	lcd_wcom(0xC2);				//A
	lcd_wdat(0x41);
	lcd_wcom(0xC3);				//T
	lcd_wdat(0x54);
	lcd_wcom(0xC4);				//E
	lcd_wdat(0x45);
	
	lcd_wcom(0xC5);				//显示冒号 在第二排第5位
	lcd_wdat(0x3A);				//显示冒号
	
	lcd_wcom(0xC8);				//显示冒号	在第二排第8位
	lcd_wdat(0x2F);				//显示冒号
	
	lcd_wcom(0xCB);				//显示冒号	在第二排第11位
	lcd_wdat(0x2F);				//显示冒号
}

void main(){
	uchar i = 0;
	init_int();
	lcdinitshow();
	
	while(1) {
		keyscan();
	}
	
}
