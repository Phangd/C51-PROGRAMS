#include <reg52.h>
#define key_state0 0
#define key_state1 1
#define key_state2 2
typedef unsigned char uint8;
typedef unsigned int uint16;

sbit key = P3^2;
// uint8 table[8];
uint8 key_value;
uint8 flag,i,dat;
bit flag1;								//控制是否开始流水
//uint8 num;			
			
void init(){			
	TMOD = 0x21;						//定时器1工作在方式2，八位自动重装
	TH1 = 0xfd;			
	TL1 = 0xfd;			
	TR1 = 0xfd;							//开启定时器1
	TH0 = 0x4C;							//50ms
	TL0 = 0x00;			
	TR0 = 1;			
	ET0 = 1;			
	SM0 = 0;							
	SM1 = 1;							//串口工作方式1
	EA = 1;								//开总中断
	ES = 1;								//开串口中断
}

void scankey(){
	static uint8 key_state;
	switch(key_state){
		case key_state0:
			if(!key) key_state = key_state1;
			break;
		case key_state1:
			if(!key){
				REN = ~REN;				//允许/禁止接收上位机数据
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

void main(){
	init();
	P1 = 0xff;
	while(1){
		if(!REN) P1 = 0xff;				//不接收上位机数据时，关闭所有灯
		if(flag){
			ES = 0;						//暂时关闭串口中断，防止在处理数据时再次发生串口中断
			// for(i=0;i<8;i++){		//回传多位数据
				// SBUF=table[i];		//发送一位
				// while(!TI);			//检测是否发送完毕，发送完毕后自动置1
				// TI=0;				//将发送完毕的标志位清零
			// }
			SBUF = dat;
			while(!TI);
			TI = 0;
			ES=1;						//重新打开串口中断
			flag=0;	
			//num=0;					//清零接收计数
		}	
	}	
}	
void ser()interrupt 4{					//串口中断服务程序
	if(RI){	
		RI = 0;							//中断标志位
		//table[num++] = SBUF;	
		dat = SBUF;						//将接收到的数据存入dat中
		P1 = SBUF;						//将收到的16进制数赋给P1
		//if(num == 8) 					//收满8位数据，开始回传
			flag=1;
	}
}

void timer0_isr() interrupt 1 using 0{
	TH0 = 0xDC;			//10ms
	TL0 = 0x00;
	scankey();
}
