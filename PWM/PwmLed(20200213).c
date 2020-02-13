#include <reg52.h>

typedef unsigned char uint8;
typedef	unsigned int uint16;
typedef	unsigned long uint32;

sbit led1 = P1^0;
sbit led2 = P1^1;
sbit led3 = P1^2;
sbit led4 = P1^3;
sbit led5 = P1^4;
sbit led6 = P1^5;
sbit led7 = P1^6;
sbit led8 = P1^7;

uint8 highTH = 0;
uint8 highTL = 0;
uint8 lowTH  = 0;
uint8 lowTL  = 0;
uint8 duty[19] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};
uint8 index = 0;
uint32 tmp;
uint16 high,low;
bit flag;

void ConfigPWM(uint16,uint8);
void ClosePWM();
void modify_duty(uint8 dc);

void main()
{	
	P1 = 0xFF;
	ConfigPWM(100,20);
	while(1)
	{
		// for(i=0; i<100; i++)			//20占空比(亮)
		// {
			// if(i<20)
			// {
				// led1 = 0;
			// }
			// else
			// {
				// led1 = 1;			//灭 80占空比
			// }
		// }
	}
}

void ConfigPWM(uint16 fr, uint8 dc){			//fr频率，dc占空比
	tmp = (11059200/12)/fr;				//tmp为对应赫兹需要的总计数次数 tmp 相当于 (65536-x)
	high = (tmp * dc)/100;				//high为高电平需要的计数次数
	low = tmp - high;					//low为低电平需要的计数次数
	high = 65536 - high;				//高电平对应的中断初值
	low = 65536- low;					//低电平对应的中断初值  
	//这里的处理方法是进入中断服务程序后才改变电位,比如100HZ 高电平20%占空比,对应的总时间是10MS,高电平的时间
	//应为2MS,低电平的时间应该为8MS,算出11.0592单片机频率下要产生100HZ方波总共需要记多少数(TMP)再进入中断服务程序,
	//然后反转电平就可以产生方波,同理可以把TMP分为HIGH和LOW,这样就能产生对应占空比的PWM波。算出来HIGH即高电平
	//需要记的次数后,就可以得出产生高电平中断的初值。同理可以得出低电平中断的初值。比如需要高电平2MS+低电平8MS
	//先在中断初始化里面把口线值1,并赋高电平对应的初值,经过2MS后进入中断服务程序,就会把低电平对应的初值赋给TH0和TL0
	//然后经过8MS再次进入中断服务程序再次置高电平的初值。这样循环就能产生高电平占空比为20的PWM波。
	highTH = high/256;
	highTL = high%256;
	lowTH = low/256;
	lowTL = low%256;
	
	TMOD = 0x11;
	EA = 1;
	TH0 = highTH;
	TL0 = highTL;
	ET0 = 1;
	TR0 = 1;
	led1 = 1;
	
	TH1 = 0x28;				//timer1 60ms中断一次,用来改变LED的占空比
	TL1 = 0x00;
	ET1 = 1;
	TR1 = 1;
}

void closePWM(){
	TR0 = 0;
	ET0 = 0;
	led1 = 1;
}

void timer0_isr()interrupt 1 using 0{
	if(led1 == 1){
		TH0 = lowTH;
		TL0 = lowTL;
		
		led1 = 0;
	}
	else{
		TH0 = highTH;
		TL0 = highTL;
		
		led1 = 1;
	}
}

void timer1_isr()interrupt 3 using 1{
	TH1 = 0x28;
	TL1 = 0x00;
	modify_duty(duty[index]);
	
	if(0 == flag)   //  flag=0时，表示呼吸灯由暗变亮
    {
        index++;
        if(index >=18)
        {
            flag = 1;
        }
    }
    else
    {
        index--;
        if(index <=0)
        {
            flag=0;
        }
    }
}

void modify_duty(uint8 dc){
	high = (tmp*dc)/100;
	low = tmp - high;
	high = 65536 - high;
	low = 65536 - low;
	highTH = high/256;
	highTL = high%256;
	lowTH = low/256;
	lowTL = low%256;
}
