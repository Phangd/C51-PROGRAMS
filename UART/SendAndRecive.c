#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;

uint8 flag,a,i;
uint8 code table[]="I get ";

void init(){
	TMOD = 0x20;			//定时器1工作在方式2，八位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;				//开启定时器1
	
	SM0 = 0;				
	SM1 = 1;				//串口工作方式1
	REN = 1;				//接收允许
	EA = 1;					//开总中断
	ES = 1;					//开串口中断
}

void main(){
	init();
	while(1){
		if(flag){
			ES = 0;			//暂时关闭串口中断，防止在处理数据时再次发生串口中断
			for(i=0;i<6;i++){
				SBUF=table[i];	//将I get放入发送寄存器
				while(!TI);		//检测是否发送完毕，发送完毕后自动置1
				TI=0;			//将发送完毕的标志位清零
			}
			SBUF=a;				//将接受到的值发回给主机
			while(!TI);			
			TI=0;
			ES=1;				//重新打开串口中断
			flag=0;
		}
	}
}
void ser()interrupt 4{		//串口中断服务程序
	RI = 0;					//中断标志位
	a = SBUF;				//将接收到的数据存入a中
	flag=1;					
}



/*状态机按键扫描+串口发送字符串，按一下发送一次*/
/*
#include <reg51.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
#define key_state0 0
#define key_state1 1
#define key_state2 2
sbit key = P3^2;
uint8 key_value;
bit flag;

uint8 Buf[]="hello world!\n";
 
void delay(uint16 n)
{
	while (n--);
}
 
//波特率为9600//
void UART_init(void)
{
    SCON = 0x50;        //串口方式1
    TMOD = 0x21;        //定时器使用方式2自动重载
    TH1 = 0xFD;    		//9600波特率对应的预设数，定时器方式2下，TH1=TL1
    TL1 = 0xFD;
	TH0 = 0x4C;			//50ms
	TL0 = 0x00;
    TR1 = 1;			//开启定时器，开始产生波特率
	TR0 = 1;
	ET0 = 1;
	EA  = 1;
}
 
//发送一个字符//
void UART_send_byte(uint8 dat)
{
	SBUF = dat;       	//把数据放到SBUF中
	while (!TI);		//未发送完毕就等待
	TI = 0;    			//发送完毕后，要把TI重新置0
}
 
//发送一个字符串//
void UART_send_string(uint8 *buf)
{
	while (*buf != '\0')
	{
		UART_send_byte(*buf++);
	}
}
 
void scankey(){
	static uint8 key_state;
	switch(key_state){
		case key_state0:
			if(!key) key_state = key_state1;
			break;
		case key_state1:
			if(!key){
				UART_send_string(Buf);
				delay(20000);
				key_state = key_state2;
			}
			else{
				key_state = key_state0;
			}
			break;
		case key_state2:
			if(key){
				key_state = key_state0;
			}
			break;
		default:break;		
	}
} 
 
void main()
{
	UART_init();
	while(1)
	{
		if(flag){
			scankey();
		}
	}
}

void timer0_isr() interrupt 1 using 0{
	TH0 = 0xDC;			//10ms
	TL0 = 0x00;
	flag = 1;
}
*/
