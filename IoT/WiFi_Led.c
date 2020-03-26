#include<reg52.h>

typedef unsigned char uint8;
typedef unsigned int uint16;
uint8 i,j,temp;
bit flag;
uint8 receive[4];
uint8 code table[]="AT+CIPMUX=1\r\n";
uint8 code table1[]="AT+CIPSERVER=1,8080\r\n";

void shortdelay(uint16 n){		//短延时
	uint16 i,j;
	for(i=n;i>0;i--){
		for(j=110;j>0;j--){;}
	}
}

void connect_init()
{
	uint8 a=0,b=0;
    while(table[a]!='\0')
    {
        SBUF=table[a];		//写入要发送的字符
        while(!TI);			//等待发送完成
        TI=0;				//发送
        a++;
    }
    shortdelay(50);			//延时函数
    while(table1[b]!='\0')
    {
		SBUF=table1[b];
		while(!TI);			//等待发送完成
		TI=0;
		b++;
    }
}

void int_init(){			//开启定时器1,产生9600波特率
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	TR1 = 1;
	
	PCON = 0x00;			//波特率不加倍
	SM0 = 0;				//串口工作方式
	SM1 = 1;	
	REN = 1;				//串口接收允许
	EA = 1;					//打开全局中断
	ES = 1;					//允许串口中断
}

void Uart() interrupt 4
{
    temp=SBUF;				//取出接收到的数据
    RI=0;					//清除接收中断标志位
	if(temp==':'||i>0)		//i是全局变量
	{
		receive[i]=temp;		//receive数组是全局变量
		i++;
		if(temp=='\n')
		{
			i=0;
		}
	}
}

void main(){
	int_init();
	connect_init();
	while(1){
		if(receive[1]=='1'){
			P1 = 0x00;
		}
		else P1 = 0xff;
	}
}

