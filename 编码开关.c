    volatile bit f_cw_buf,f_anticw_buf,f_change,flag1,flag2,flag3,flag4;
    volatile unsigned char temp_setup;

void key_scan(){
    if(sw_a != f_cw_buf){
        f_cw_buf = sw_a;
        flag1 = 1;                  //flag1 正转标志位
        f_change = 1;               //变化标志位,编码开关拧到一定程度才会被置1,防止编码开关静止时反复进入判断
    }
    else if(sw_b != f_anticw_buf){
        f_anticw_buf = sw_b;
        flag2 = 1;                  //flag2 反转标志位
        f_change = 1;               
    }
    if(f_change){                   //初次改变后,判断正反转,在将编码器拧到某个卡位的时候,两脚电平变化有一个8ms的相位差
        if(flag1 && !flag2 && !flag4){
            flag3 = 1;              //如果flag4还是0,说明此时反转波形还没到,正转波形先,说明是顺时针扭
        }                           //哪个波形先到,会先改变f_change和flag1/2的值,从而进入判断,然后改变flag3/4的值.
        else if(!flag1 && flag2 && !flag3){
            flag4 = 1;              //后面的波形到达时,flag3/flag4其中一个已经置1,就不会重复进入正反转的判断
        }
        if(sw_a == sw_b && flag1 && flag2){         //扭到卡位后,两脚的电平是相等的,此时如果扭回中间,然后再扭回卡位,波形上就会出现一个脚电平不变,另一个脚不断改变,程序就会因为sw_a=sw_b反复相应
            if(flag3){                              //加入flag1和flag2判定条件之后,必须要两脚电平都发生变化才能响应为一次有效扭动。
                temp_setup++;                       //正转
            }
            else if(flag4){
                temp_setup--;                       //反转
            }
            f_change = 0;
            flag1 = flag2 = flag3 = flag4 = 0;      //执行完一次有效的扭动之后,将所有的标志位清零
        }
    }
}
