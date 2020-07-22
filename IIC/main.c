/***********************************************************
IIC start
***********************************************************/
void iic_start(){
    SCL = 0;
    SDA = 1;
    SCL = 1;
    SDA = 0;
    SCL = 0;
}

/***********************************************************
IIC stop
***********************************************************/
void iic_stop(){
    SCL = 0;
    SDA = 0;
    SCL = 1;
    SDA = 1;
    SCL = 0;
}
                           
/***********************************************************
IIC send
***********************************************************/
bit iic_send(unsigned char data){
    unsigned char i,j;
    for(i=0; i<8 ;i++){
        SCL = 0;
        if(data & 0x80){
            SDA = 1;
        }
        else{
            SDA = 0;
        } 
        SCL = 1;
        data <<= 1;
    }
    SCL = 0;
    TRISC5 = 1;
    SCL = 1;
    j = SDA;
    SCL = 0;
    TRISC5 = 0;
    return j;
}
