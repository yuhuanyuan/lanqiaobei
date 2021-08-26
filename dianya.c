#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char 
#define uint unsigned int	
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1	

sbit SCL=P2^0;
sbit SDA=P2^1;

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar yi,er,san,si,wu ,liu,qi,ba,T;
uchar dianya;

void delayms(int ms);
void Delay50ms();
void allinit();
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);



void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt);
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
void AD_write(uchar add,uchar dat);
uchar AD_read(uchar add);

void select(unsigned char n);
void close();
void Timer0Init(void)	;
void rbinit();

sbit relay=P0^4;
sbit buzz=P0^6;
sbit l1=P0^0;
sbit l2=P0^1;
sbit l3=P0^2;

void select(unsigned char n)
{
	P2=P2&0x1f;
	P2=P2|(n<<5);
}
void close()
{
	P2=P2&0x1f;
}


void rbinit()
{
	relay=0;
	buzz=0;
	select(5);
	close();
}
void Timer0Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	EA=1;ET0=0;
}


void main()
{
	allinit();
	Timer0Init();
	yi=11;er=11;san=11;si=11;wu=11;liu=11;qi=11;ba=11;
	while(1)
	{

		dianya=AD_read(0X03);
    
		
		liu=dianya/100;qi=dianya%100/10;ba=dianya%10;
		if(dianya<50)
		{	
			ET0=0;
			allinit();
      P2=0X80;P0=0XFE;
    }			
			
		if((dianya<100)&&(dianya>50))
			{
			ET0=0;
			allinit();
				P2=0X80;P0=0XFD;

			}
		if(dianya>100)
			{
         ET0=1;
				
      
			}

		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
	
	}


}



void isr0() interrupt 1
{
	T++;	
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;	
	if(T==50)
	{
		T=0;

	  l3=~l3;		P2=0X80;P00=1;P01=1;P03=1;P04=1;P05=1;P06=1;P07=1;
	}
	
}
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}
//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop;
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}







uchar AD_read(uchar add)
{
	uchar temp;
	
	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0X91);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_Stop();
	
	
	return temp;
	
}

void AD_write(uchar add,uchar dat)
{

	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
  IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
	

}


void DAC(uchar dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();

}

void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=845;j>0;j--);
}

void allinit()
{
	P2=0XA0;
	P0=0X00;//关闭蜂鸣器，继电器
	
	P2=0X80;
	P0=0XFF;//关闭LED灯
	
	P2=0XC0;
	P0=0XFF;//选择所有数码管
	P2=0XFF;
	P0=0XFF;//关闭所有数码管
}

void display1(uchar yi,uchar er)
{
		P2=0XC0;//打开位选573   U8
		P0=0X01;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[yi];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X02;//选择第二个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[er];
		delayms(1);
}	

void display2(uchar san,uchar si)
{
		P2=0XC0;//打开位选573   U8
		P0=0X04;//选择第三个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[san];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X08;//选择第四个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[si];
		delayms(1);
}

void display3(uchar wu,uchar liu)
{
		P2=0XC0;//打开位选573   U8
		P0=0X10;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[wu];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X20;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[liu];
		delayms(1);
}

void display4(uchar qi,uchar ba)
{
		P2=0XC0;//打开位选573   U8
		P0=0X40;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[qi];
		delayms(1);
		
		P2=0XC0;//打开位选573   U8
		P0=0X80;//选择第一个数码管
		P2=0XFF;//打开段选573   U7
		P0=tab[ba];
		delayms(1);
}



