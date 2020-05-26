/*------------------------------------------------------------------*/
/* --- STC MCU Limited ---------------------------------------------*/
/* --- STC12C5Axx Series MCU PCA module output PWM wave Demo -------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966----------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/
/*********************************************************************
*		                		 呼吸灯							     *
实现现象：上电之后,P1.3 P1.4 每5MS占空比增加1%,加到100%之后开始递减到0%
				P1.3与P1.4的波形互补(相反)
注意事项：																				  
*********************************************************************/
#include "reg51.h"
#include "intrins.h"

#define FOSC    11059200L

typedef unsigned char BYTE;
typedef unsigned int WORD;

/*Declare SFR associated with the PCA */
sfr CCON        =   0xD8;           //PCA control register
sbit CCF0       =   CCON^0;         //PCA module-0 interrupt flag
sbit CCF1       =   CCON^1;         //PCA module-1 interrupt flag
sbit CR         =   CCON^6;         //PCA timer run control bit
sbit CF         =   CCON^7;         //PCA timer overflow flag
sfr CMOD        =   0xD9;           //PCA mode register
sfr CL          =   0xE9;           //PCA base timer LOW
sfr CH          =   0xF9;           //PCA base timer HIGH
sfr CCAPM0      =   0xDA;           //PCA module-0 mode register
sfr CCAP0L      =   0xEA;           //PCA module-0 capture register LOW
sfr CCAP0H      =   0xFA;           //PCA module-0 capture register HIGH
sfr CCAPM1      =   0xDB;           //PCA module-1 mode register
sfr CCAP1L      =   0xEB;           //PCA module-1 capture register LOW
sfr CCAP1H      =   0xFB;           //PCA module-1 capture register HIGH
sfr PCAPWM0     =   0xf2;
sfr PCAPWM1     =   0xf3;

sbit led1 = P1^0;
sbit led4 = P1^3;

unsigned char num=0x00;
unsigned char num1=0xfe;
unsigned int delay1;
bit flag,flag1,flag2;

void pwm_set(unsigned char x,unsigned char y){
	CCAP0H=x; //设置比较值
	CCAP0L=x;
	CCAP1H=y;
	CCAP1L=y;
}

bit delay1ms()
{
	unsigned int x,y;
for(x=1; x>0; x--)
  for(y=920; y>0; y--);
	return 1;
}


void pwm_init(){
	CCON = 0;                       //Initial PCA control register
                                    //PCA timer stop running
                                    //Clear CF flag
                                    //Clear all module interrupt flag
    CL = 0;                         //Reset PCA base timer
    CH = 0;
    CMOD = 0x02;                    //Set PCA timer clock source as Fosc/2
                                    //Disable PCA timer overflow interrupt
    CCAP0H = CCAP0L = 0x00;         //PWM0 port output 0% duty cycle square wave
    CCAPM0  = 0x42;                  //PCA module-0 work in 8-bit PWM mode and no PCA interrupt

    CCAP1H = CCAP1L = 0xff;         //PWM1 port output 0% duty cycle square wave
    CCAPM1 = 0x42;                  //PCA module-1 work in 8-bit PWM mode and no PCA interrupt

    CR = 1;                         //PCA timer start run
}

void tmr0_init(){
	EA = 0;
	TF0 = 0;
	TMOD = 0X01;
	TH0 = 0XEE;						//5MS
	TL0 = 0X00;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void isr_timer0 () interrupt 1 using 0{
	TH0 = 0XEE;						//5MS
	TL0 = 0X00;
	if(!flag){
		if(++num > 0xfe) flag = 1;
	}
	else{
		if(--num < 0x01) flag = 0;
	}
	if(!flag1){
		if(++num1 > 0xfe) flag1 = 1;
	}
	else{
		if(--num1 < 0x01) flag1 = 0;
	}
}
void main()
{
    pwm_init();
	tmr0_init();
    while (1){
		pwm_set(num,num1);	
	}
}

