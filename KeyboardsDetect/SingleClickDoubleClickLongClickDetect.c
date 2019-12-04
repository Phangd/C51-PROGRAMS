#include <reg52.h>
typedef	unsigned int uint16;
typedef	unsigned char uint8;
#define no_key 0xff
#define key_state0 0
#define key_state1 1
#define key_state2 2
#define key_state3 3
#define single_click 0
#define double_click 1
#define long_click 2

sbit key1 = P3^2;

sbit led_click = P1^0;
sbit led_double = P1^1;
sbit led_long = P1^2;
bit int_flag;
uint8 cnt = 0;

uint8 key_scan();	
uint8 key_jud();

uint8 key_scan(){
	static uint8 key_state;
	static uint8 key_timer_cnt1 = 0;
	uint8 key_value = no_key;

	switch(key_state){
		case key_state0:
			if(!key1) key_state = key_state1;	//按键按下，切换到状态1
			break;
		case key_state1:
			if(!key1){
				key_timer_cnt1 = 0;				//清除长按计时
				key_state = key_state2;
			}
			else{
				key_state = key_state0;			//松开按键，状态切回0
			}
			break;
		case key_state2:
			if(key1){							//按键松开，1次单击
				key_value = single_click;
				key_state = key_state0;
			}
			else if(++key_timer_cnt1 >= 20){	//长按计时
				key_value = long_click;			//大于1s，判定为长按
				key_state = key_state3;			//跳到松手检测
			}
			break;
		case key_state3:
			if(key1){
				key_state = key_state0;			//松手，状态切回0
			}
			break;
		default:break;
	}
	return key_value;
}

uint8 key_jud(){
	static uint8 key_state;
	static uint8 key_timer_cnt2 = 0;
	uint8 key_value = no_key;
	uint8 key_value1;
	key_value1 = key_scan();
	
	switch(key_state){
		case key_state0:
			if(key_value1 == single_click){		//1次单击，跳到状态2检测是否还有第二次单击
				key_timer_cnt2 = 0;				//清除单击间隔计时变量
				key_state = key_state1;			//换到状态1
			}
			else{
				key_value = key_value1;			//非单击状态(长按或无键按下)直接将状态送出
			}
			break;
		case key_state1:
			if(key_value1 == single_click){		//第二次单击
				key_value = double_click;		//将双击状态送出
				key_state = key_state0;
			}
			else if(++key_timer_cnt2 >= 10){	//两次单击之间计时
				key_value = single_click;		//大于0.5s则判定为单击
				key_state = key_state0;			//回到状态0
			}
			break;
		default:break;
	}
	return key_value;							//送出判定结果
}

void timer0init(){
	EA = 0;
	TR0 = 0;
	TMOD = 0x01;
	TH0 = 0x4c;			//50ms中断
	TL0 = 0x00;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void timer0_isr() interrupt 1 using 0 {
	TH0 = 0x4c;
	TL0 = 0x00;
	int_flag = 1;
}

void main(){
	uint8 key;
	timer0init();
	while(1){
		if(int_flag){
			int_flag = 0;
			key = key_jud();
			switch(key){
				case single_click: led_click = !led_click;break;		//单击指示灯
				case double_click: led_double = !led_double;break;		//双击指示灯
				case long_click: led_long = !led_long;break;			//长按指示灯
				default:break;
			}
		}
	}
}
