#include<STC15F2K60S2.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int

uchar code tab[]={
	                 0xc0,//0
	                 0xf9,//1
	                 0xa4,//2
	                 0xb0,//3
	                 0x99,//4
	                 0x92,//5
	                 0x82,//6
	                 0xf8,//7
	                 0x80,//8
	                 0x90,//9
	                 0XBF,//-
	                 0XFF
	                };
uchar yi,er,san,si,wu,liu,qi,ba;
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */
uchar shidu;
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}    


#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1



void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void delayms(uint ms)	;
void delay1ms();
void allinit();
void keyscan();
void IIC_Start(void);
unsigned char  AD_read(unsigned char add);
void IIC_Stop(void);
void IIC_Ack(bit ackbit);
bit IIC_WaitAck(void);
void IIC_SendByte(unsigned char byt);
unsigned char IIC_RecByte(void);

void main()
{
  allinit();
	yi=10;er=0;san=0;si=10;wu=1;liu=1;qi=1;ba=1;
	while(1)
	{

		shidu=AD_read(0x03)*0.39;
	  P2=0X80;P0=0XFd;
		liu=shidu/100;qi=shidu%100/10;ba=shidu%10;
	 display1(yi,er);
   display2(san,si);
   display3(wu,liu);
   display4(qi, ba);
	}

}

void allinit()
{
  P2=0X80;P0=0XFF;//LED
	P2=0XA0;P0=0X00;//BEE
	P2=0XC0;P0=0XFF;P2=0XFF;P0=0XFF;//数码管

}


void display1(uchar yi,uchar er)
{
  P2=0XC0;
	P0=0X01;
	P2=0XFF;
	P0=tab[yi];
	delay1ms();
	
	P2=0XC0;
	P0=0X02;
	P2=0XFF;
	P0=tab[er];
	delay1ms();

}
void display2(uchar san,uchar si)
{
  P2=0XC0;
	P0=0X04;
	P2=0XFF;
	P0=tab[san];
	delay1ms();
	
	P2=0XC0;
	P0=0X08;
	P2=0XFF;
	P0=tab[si];
	delay1ms();

}
void display3(uchar wu,uchar liu)
{
  P2=0XC0;
	P0=0X10;
	P2=0XFF;
	P0=tab[wu];
	delay1ms();
	
	P2=0XC0;
	P0=0X20;
	P2=0XFF;
	P0=tab[liu];
	delay1ms();

}
void display4(uchar qi,uchar ba)
{
  P2=0XC0;
	P0=0X40;
	P2=0XFF;
	P0=tab[qi];
	delay1ms();
	
	P2=0XC0;
	P0=0X80;
	P2=0XFF;
	P0=tab[ba];
	delay1ms();

}
void delay1ms()		//@11.0592MHz
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
void delayms(uint ms)	
{
  uint i,j;
	for(i=ms;i>0;i--)
	  for(j=845;j>0;j--);
}	
void keyscan()
{
  if(P30==0)
	{
		delayms(5);
    if(P30==0)
		{
		  yi=9;
		}while(!P30);			
	}
 if(P31==0)
	{
		delayms(5);
    if(P31==0)
		{
		  er=9;
		}while(!P31);			
	}
 if(P32==0)
	{
		delayms(5);
    if(P32==0)
		{
		  san=9;
		}while(!P32);			
	}
if(P33==0)
	{
		delayms(5);
    if(P33==0)
		{
		  si=9;
		}while(!P33);			
	}


}



//总线启动条件
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

//应答位控制
void IIC_Ack(bit ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	SDA = 1; 
	somenop;
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




//以上为引用部分
unsigned char  AD_read(unsigned char add)
{
  unsigned char temp;
	
  IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_Stop();
	
  IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
  temp=IIC_RecByte();
  IIC_Stop();
	
	return temp;

}



