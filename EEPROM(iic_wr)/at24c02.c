#include <reg52.h>
#include "intrins.h"
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}
typedef unsigned char uint8;
typedef unsigned int uint16;
//************************************************端口声明

sbit sda = P2^0;
sbit scl = P2^1;
	
sbit ls138A = P2^2;					//38译码器的A B C,用来选择需要显示的数码管(位选)
sbit ls138B = P2^3;
sbit ls138C = P2^4;

sbit key1 = P3^1;
sbit key2 = P3^0;
sbit key3 = P3^2;
sbit key4 = P3^3;

//************************************************变量 函数声明
uint8 code seg[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint8 com[4];
uint16 temp;

void iic_start();
void iic_stop();
bit wait_ack();
void iic_wr(uint8 dat);
uint8 iic_rd();
void write_byte(uint8 add,uint8 dat);
uint8 read_byte(uint8 add);
void delay(uint16 z);								//延时
void scankey();
void display();
//************************************************

void delay(uint16 z){
	uint8 x,y;
	for(x=z;x>0;x--){
		for(y=110;y>0;y--);
	}
}

void iic_start(){
	sda = 1;
	somenop;
	scl = 1;
	somenop;
	sda = 0;
	somenop;
	scl = 0;					//拉低scl,占用scl准备发送数据
	somenop;
}

void iic_stop(){
	sda = 0;
	somenop;
	scl = 1;
	somenop;
	sda = 1;
}

bit wait_ack(){
	uint8 max_time = 0;
	// scl = 1;					//释放SCL和SDA
	// somenop;
	// sda = 1;
	// somenop;					//iic_wr后scl和sda都已经释放
	while(sda){
		if(++max_time > 200){	//等待时间超过200,视为无应答,发送停止信号
			scl = 0;
			iic_stop();
			return 0;
		}
	}										
		scl = 0;							//应答正常,拉低scl形成第9个时钟脉冲
		return 1;
}

void iic_wr(uint8 dat){
	uint8 i;
	for(i=0;i<8;i++){
		if(dat & 0x80){		
			sda = 1;
		}
		else{
			sda = 0;
		}
		dat = dat << 1;
		somenop;
		scl = 1;
		somenop;
		scl = 0;
	}
	scl = 1;					//释放SCL和SDA
	somenop;
	sda = 1;
	somenop;
}

uint8 iic_rd(){
	uint8 i;
	uint8 dat;
	
	scl = 0;					//在改变sda前先确保scl为低电平
	somenop;
	sda = 1;					//主机释放sda,以便从机可以改变sda
	somenop;
	
	for(i=0;i<8;i++){
		scl = 1;				//拉高scl,接收从机发来的sda
		somenop;
		dat <<= 1;
		dat |= sda;
		scl = 0;
		somenop;				//拉低scl,让从机可以改变sda的电平
	}
	return dat;
}

uint8 read_byte(uint8 add){
	uint8 dat;
	
	iic_start();
	iic_wr(0xa0);				//写入设备地址 最后一位为0代表主机要写入数据到从机
	wait_ack();
	iic_wr(add);				//写入要读数据的地址
	wait_ack();
	iic_start();				
	iic_wr(0xa1);				//写入设备地址 最后一位为1代表主机要读从机数据
	wait_ack();
	dat = iic_rd();				//保存读出的数据
	iic_stop();					//结束读数据
	return dat;
}

void write_byte(uint8 add,uint8 dat){
	iic_start();
	iic_wr(0xa0);				//写入设备地址
	wait_ack();
	iic_wr(add);				//写入存放数据的地址
	wait_ack();
	iic_wr(dat);				//写入数据
	wait_ack();
	iic_stop();					//结束写数据
}

void scankey(){
	if(!key1){
		delay(1000);
		if(!key1){
			write_byte(1,temp);
			while(!key1);
		}
	}
	if(!key2){
		delay(1000);
		if(!key2){
			temp = read_byte(1);
			while(!key2);
		}
	}
	if(!key3){
		delay(1000);
		if(!key3){
			if(temp < 65536) temp++;
			while(!key3);
		}
	}
	if(!key4){
		delay(1000);
		if(!key4){
			if(temp > 0) temp--;
			while(!key4);
		}
	}
}

void display(){
	uint8 i;
	com[0] = seg[temp/1000];
	com[1] = seg[temp%1000/100];
	com[2] = seg[temp%1000%100/10];
	com[3] = seg[temp%1000%100%10];
	for(i=0;i<4;i++){
		switch(i){
			case 0: ls138A = 0;ls138B = 0;ls138C = 0;break;
			case 1: ls138A = 1;ls138B = 0;ls138C = 0;break;
			case 2: ls138A = 0;ls138B = 1;ls138C = 0;break;
			case 3: ls138A = 1;ls138B = 1;ls138C = 0;break;
		}
		P0 = com[3-i];
		delay(300);
		P0 = 0x00;				//消隐
	}	
}

void main(){
	scl = 1;
	sda = 1;					//IIC初始化
	temp = 0;
	
	while(1){
		scankey();
		display();
	}
}
