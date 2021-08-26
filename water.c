##include<STC15F2K60S2.H>
#include<intrins.h>


#define uchar unsigned char
#define uint unsigned int

sbit SDA = P2^1;  
sbit SCL = P2^0;  

uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar yi,er,san,si,wu,liu,qi,ba;
extern unsigned char shijian[]={0,30,8,0,0,0,0};

void allinit();
void Delay1ms();
void Delay5ms();
void keyscan();
void somenop();
void Timer0Init(void);
unsigned char IIC_RecByte(void);
void IIC_SendByte(unsigned char byt);
bit Ack(void);
void IIC_Stop(void);
void IIC_Start(void);
uchar iicread(uchar add);
void iicwrite(uchar add,uchar dat);

void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);

uchar T;
uint ML10,money;//99990ml 9999ml10

void main()
{
	uchar LIGHT;
  allinit();
  Timer0Init();
	yi=11;er=0;san=5;si=0;wu=0;liu=1;qi=0;ba=0;
	while(1)
	{

		wu=ML10/1000;liu=ML10%1000/100;qi=ML10%100/10;ba=ML10%10;
	  LIGHT=iicread(0x01);
		if(LIGHT<64)
		{
		  P2=0X80;
			P0=0X00;
		
		}
		else
		{
		  P2=0X80;
			P0=0Xff;
		
		}
		keyscan();
	 display1(yi,er);
	 display2(san,si);
	 display3(wu,liu);
	 display4(qi,ba);



	}

}
void keyscan()
{
  if(P30==0)
	{
		Delay5ms();
	   if(P30==0)
		 {
	      EA=1;ET0=1;
        P2=0Xa0;P0=0X10;	
        wu=0;liu=0;qi=0;ba=0;			 //3210ml10 32100ml 32.1L
		 
		 }while(!P30);
	 
	}
   if(P31==0)
	{
	   if(P31==0)
		 {
	      EA=0;ET0=0;
        P2=0XA0;P0=0X00;	
        money=ML10*0.5;			 
		    wu=money/1000;liu=money%1000/100;qi=money%100/10;ba=money%10;
		 }while(!P31);
	 
	}
}
void Timer0Init(void)		//5毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void tm0_isr() interrupt 1
{
   T++;
	if(T==20)
	{
	 T=0;
		ML10=ML10+1;
		wu=ML10/1000;liu=ML10%1000/100;qi=ML10%100/10;ba=ML10%10;
		if(ML10==9999)
		{
		  P2=0XA0;P0=0X00;
			
		}
	}
}

void somenop()
{ 
	 uchar i=50;
	 do
		 {
     _nop_(); 
		 }while(i--);
}    

void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop();
	SDA = 0;
	somenop();
	SCL = 0;	
}

void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop();
	SDA = 1;
}


bit Ack(void)
{
	SDA = 1;
	somenop();
	SCL = 1;
	somenop();
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
		somenop();
		SCL = 1;
		byt <<= 1;
		somenop();
		SCL = 0;
	}
}


unsigned char IIC_RecByte(void)//readbyte
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		somenop();
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop();
	}
	return da;
}

void iicwrite(uchar add,uchar dat)
{
  IIC_Start();
	IIC_SendByte(0x90);
	Ack();
	IIC_SendByte(add);
	Ack();
	IIC_SendByte(dat);
	Ack();
	IIC_Stop();
	
}

uchar iicread(uchar add)
{
	uchar temp;
	
  IIC_Start();
	IIC_SendByte(0x90);
	Ack();
	IIC_SendByte(add);
	Ack();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	Ack();
	temp=IIC_RecByte();
	IIC_Stop();
	
	temp=0.39*temp;
	return temp;
}

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void allinit()
{
  P2=0XA0;
	P0=0X00;//关闭蜂鸣器
	
	P2=0X80;
	P0=0XFF;//关闭led
	
	P2=0XC0;
	P0=0XFF;//选择所有数码管
	P2=0XFF;
	P0=0XFF;//关闭所有数码管
}
void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void Delay5ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 54;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}


void display1(uchar yi,uchar er)
{
  P2=0XC0;
	P0=0X01;
	P2=0XFF;
	P0=tab[yi];
	Delay1ms();
	
	P2=0XC0;
	P0=0X02;
	P2=0XFF;
	P0=tab[er]&0x7f;
	Delay1ms();
}
void display2(uchar san,uchar si)
{
  P2=0XC0;
	P0=0X04;
	P2=0XFF;
	P0=tab[san];
	Delay1ms();
	
	P2=0XC0;
	P0=0X08;
	P2=0XFF;
	P0=tab[si];
	Delay1ms();
}
void display3(uchar wu,uchar liu)
{
  P2=0XC0;
	P0=0X10;
	P2=0XFF;
	P0=tab[wu];
	Delay1ms();
	
	P2=0XC0;
	P0=0X20;
	P2=0XFF;
	P0=tab[liu]&0x7f;
	Delay1ms();
}
void display4(uchar qi,uchar ba)
{
  P2=0XC0;
	P0=0X40;
	P2=0XFF;
	P0=tab[qi];
	Delay1ms();
	
	P2=0XC0;
	P0=0X80;
	P2=0XFF;
	P0=tab[ba];
	Delay1ms();
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
	P0=0X00;//?????,???
	
	P2=0X80;
	P0=0XFF;//??LED?
	
	P2=0XC0;
	P0=0XFF;//???????
	P2=0XFF;
	P0=0XFF;//???????
}

void display1(uchar yi,uchar er)
{
		P2=0XC0;//????573   U8
		P0=0X01;//????????
		P2=0XFF;//????573   U7
		P0=tab[yi];
		delayms(1);
		
		P2=0XC0;//????573   U8
		P0=0X02;//????????
		P2=0XFF;//????573   U7
		P0=tab[er];
		delayms(1);
}	

void display2(uchar san,uchar si)
{
		P2=0XC0;//????573   U8
		P0=0X04;//????????
		P2=0XFF;//????573   U7
		P0=tab[san];
		delayms(1);
		
		P2=0XC0;//????573   U8
		P0=0X08;//????????
		P2=0XFF;//????573   U7
		P0=tab[si];
		delayms(1);
}

void display3(uchar wu,uchar liu)
{
		P2=0XC0;//????573   U8
		P0=0X10;//????????
		P2=0XFF;//????573   U7
		P0=tab[wu];
		delayms(1);
		
		P2=0XC0;//????573   U8
		P0=0X20;//????????
		P2=0XFF;//????573   U7
		P0=tab[liu];
		delayms(1);
}

void display4(uchar qi,uchar ba)
{
		P2=0XC0;//????573   U8
		P0=0X40;//????????
		P2=0XFF;//????573   U7
		P0=tab[qi];
		delayms(1);
		
		P2=0XC0;//????573   U8
		P0=0X80;//????????
		P2=0XFF;//????573   U7
		P0=tab[ba];
		delayms(1);
}



