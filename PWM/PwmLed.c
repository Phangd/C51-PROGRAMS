// /*-----------------------------------------------
// 功能：通过PWM（脉宽调制）调节LED的亮度
// ------------------------------------------------*/
// #include <reg52.h>

// sbit LED0=P1^0;

// void Delay(unsigned int t);
// /*------------------------------------------------
                    // 主函数
// ------------------------------------------------*/
// void main (void)
// {                
	// unsigned int CYCLE=600,PWM_LOW=0;//定义周期并赋值
	// while (1)         
	// {
		// LED0=1;
		// Delay(60000);        //完全熄灭后的延时
		// for(PWM_LOW=1;PWM_LOW<CYCLE;PWM_LOW++){
	//电平时间，这个循环中低电平时长从1累加到CYCLE（周期）的值，即600次
			// LED0=0;        //点亮LED  
			// Delay(PWM_LOW);//延时长度，600次循环中从1加至599
			// LED0=1;        //熄灭LED
            // Delay(CYCLE-PWM_LOW);//延时长度，600次循环中从599减至1
			// }
		// LED0=0;
		// for(PWM_LOW=CYCLE-1;PWM_LOW>0;PWM_LOW--){
			//与逐渐变亮相反的过程
            // LED0=0;
            // Delay(PWM_LOW);
            // LED0=1;
            // Delay(CYCLE-PWM_LOW);
   // }
		//主循环中添加其他需要一直工作的程序
  // }
// }
// /*------------------------------------------------
// 延时函数，含有输入参数 unsigned int t，无返回值
// unsigned int 是定义无符号整形变量，其值的范围是
// 0~65535
// ------------------------------------------------*/
// void Delay(unsigned int t){
	// while(--t);
// }



// #include <stc12c5a60s2.h>
 
// unsigned long cnt;
// unsigned int cnt_T1;
// unsigned int high;
// unsigned int low;
// unsigned char TH0_high;
// unsigned char TL0_high;
// unsigned char TH0_low;
// unsigned char TL0_low;
// unsigned char T1H1;
// unsigned char T1L1;
// unsigned char flag=0;
 
// unsigned char duty[19] = {5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95};
// unsigned char index=0;
 
// void config_Fy_and_dy(unsigned char fy, unsigned char dy);
// void config_Timer1(unsigned char ms);
// void modify_duty(unsigned char dy);
 
// sbit PWM_OUT = P2^5;
 
// void main()
// {
    // TMOD = 0X11;    //  定时器0/1都工作在方式1，是2^16计数
    // EA=1;
    // config_Fy_and_dy(100, 20);
    // config_Timer1(50);
    // while(1);
// }
 
// void config_Fy_and_dy(unsigned char fy, unsigned char dy) // 输入频率fy，占空比dy 
// {
    // cnt =  (11059200/fy)/12;    //  要产生频率为fy，定时器0所要计的数值
    // high = (cnt*dy)/100;        //  计算出高电平所要计的数值
    // low = cnt - high;   //  计算出低电平所要计的数值
    // high = 65536 - high;  //  确定定时器0，从哪个初值开始计时，记录的是高电平
    // low = 65536 - low;  //  确定定时器0，从哪个初值开始计时，记录的是低电平
    // TH0_high = (unsigned char)(high>>8);    //  高电平，给TH0预备重装初值
    // TL0_high = (unsigned char)high;         //  高电平，给TL0预备重装初值
    // TH0_low = (unsigned char)(low>>8);      //  低电平，给TH0预备重装初值
    // TL0_low = (unsigned char)low;           //  低电平，给TL0预备重装初值
    // TH0 = TH0_high; 
    // TL0 = TL0_high;
    // PWM_OUT = 1;               
    // ET0 = 1;
    // TR0 = 1;
// }
 
// void config_Timer1(unsigned char ms)			//配置timer1，MS为可调的中断间隔
// {												//timer1用于改变不同的占空比值
    // unsigned long temp;
    // temp =  11059200/12;
    // temp = (temp*ms)/1000;
    // cnt_T1 = 65536 - temp;
    // T1H1 = (unsigned char)(cnt_T1>>8);
    // T1L1 = (unsigned char)cnt_T1;
    // TH1 = T1H1;
    // TL1 = T1L1;
    // ET1=1;
    // TR1=1;
// }
 
// void modify_duty(unsigned char dy)
// {
    // high = (cnt*dy)/100;
    // low = cnt - high;
    // high = 65536 - high;
    // low  = 65536 - low;
    // TH0_high = (unsigned char)(high>>8);    //  高电平，给TH0预备重装初值
    // TL0_high = (unsigned char)high;         //  高电平，给TL0预备重装初值
    // TH0_low = (unsigned char)(low>>8);      //  低电平，给TH0预备重装初值
    // TL0_low = (unsigned char)low;           //  低电平，给TL0预备重装初值
// }
 
// void T0_Timer0() interrupt 1
// {
    // if(PWM_OUT==0)  //  准备产生高电平
    // {
        // TH0 = TH0_high;         //  高电平产生  初值重装
        // TL0 = TL0_high;
        // PWM_OUT = 1;
    // }
    // else
    // {
        // TH0 = TH0_low;         //  低电平产生  初值重装
        // TL0 = TL0_low;
        // PWM_OUT = 0;
    // }
// }
 
// void T1_Timer1() interrupt 3
// {
    // TH1 = T1H1;
    // TL1 = T1L1;
    // modify_duty(duty[index]);
    // if(0 == flag)   //  flag=0时，表示呼吸灯由暗变亮
    // {
        // index++;
        // if(index >=18)
        // {
            // flag = 1;
        // }
    // }
    // else
    // {
        // index--;
        // if(index <=0)
        // {
            // flag=0;
        // }
    // }
// }

// #include <stc12c5a60s2.h>

// typedef unsigned char u8;

// u8 code LightLevel[8] = {0,1,2,4,8,16,32,64};
// void CloseFucker();

// void main()
// {
	// u8 i,j;
	// u8 temp = 0;
	// CloseFucker();
	// P2 = 0xFF;
	// while(1)
	// {
		// for(i=0; i<64; i++)//共64个脉冲(其实就是对应的总周期)
		// {
			// for(j=0; j<8; j++)
			// {
				// if(LightLevel[j] <= i)
				// {
					// temp |= (1<<j);//小于亮度等级值置1(灭)(对应的高电平持续时间)
				// }
				// else
				// {
					// temp &= ~(1<<j);//否则清0(亮)(对应的低电平持续时间)
				// }
			// }
			// P2 = temp;
		// }
	// }
// }

// void CloseFucker()
// {
	// P2 &= 0xAF;
// }


/*
*******************************************************************************
* 文件名：main.c
* 描  述：呼吸灯
* 作  者：CLAY
* 版本号：v1.0.1
* 日  期: 
* 备  注：T0用来产生PWM波，T1来查表法改变占空比
*         不再使用while(1) 里面的做法
*******************************************************************************
*/

#include <stc15.h>

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

sbit PWMOUT = P0^0;

u32 PeriodCnt = 0;
u8 HighRH = 0;
u8 HighRL = 0;
u8 LowRH = 0;
u8 LowRL = 0;
u8 T1RH = 0;
u8 T1RL = 0;

void CloseFucker();
void ConfigPWM(u16 fr, u8 dc);
void ConfigTimer1(u16 ms);

void main()
{
	EA = 1;
	CloseFucker();
	P2 = (P2 & 0x1F) | 0x80;
	P0 = 0xFF;
 	ConfigPWM(100, 1);
	ConfigTimer1(50);	

 	while(1)
	{
//	 	ConfigPWM(100, 1);
//		ConfigTimer1(50);
	}	
}

void CloseFucker()
{
 	P2 = (P2 & 0x1F) | 0xA0;
	P0 = 0xAF;
	P2 = 0x00;
}

void ConfigPWM(u16 fr, u8 dc)
{
 	u16 high, low;
	
	PeriodCnt = (11059200/12)/fr;
	high = (PeriodCnt*dc)/100;
	low = PeriodCnt - high;
	high = 65536 - high;
	low = 65536 - low;
	HighRH = (u8)(high>>8);
	HighRL = (u8)high;
	LowRH = (u8)(low>>8);
	LowRL = (u8)low;

	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = HighRH;
	TL0 = HighRL;
	ET0 = 1;
	TR0 = 1;
	PWMOUT = 1;	
}

void AdjustPWM(u8 dc)
{
	u16 high, low;

	high = (PeriodCnt*dc)/100;
	low = PeriodCnt - high;
	high = 65536 - high;
	low = 65536 - low;
	HighRH = (u8)(high>>8);
	HighRL = (u8)high;
	LowRH = (u8)(low>>8);
	LowRL = (u8)low;
}

void ConfigTimer1(u16 ms)
{
 	u32 tmp;

	tmp = 11059200/12;
	tmp = (tmp*ms)/1000;
	tmp = 65536 - tmp;
	T1RH = (u8)(tmp >> 8);
	T1RL = (u8)tmp;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TH1 = T1RH;
	TL1 = T1RL;
	ET1 = 1;
	TR1 = 1;		
}

void InterruptTimer0() interrupt 1
{
 	if(PWMOUT == 1)
	{
	 	TH0 = LowRH;
		TL0 = LowRL;
		PWMOUT = 0;
	}
	else
	{
	 	TH0 = HighRH;
		TL0 = HighRL;
		PWMOUT = 1;
	}
}

void InterruptTimer1() interrupt 3
{
	static u8 dir = 0;
	static u8 index = 0;
	u8 code Table[] = {
		2, 3, 5, 11, 16, 25, 36, 44, 54, 66, 76, 87, 92, 96
	};
	
	AdjustPWM(Table[index]);
	
	if(dir == 0)
	{
	 	index++;
		if(index == 13)
			dir = 1;
	}
	if(dir == 1)
	{
	 	index--;
		if(index == 0)
			dir = 0;
	}
	
			
}
