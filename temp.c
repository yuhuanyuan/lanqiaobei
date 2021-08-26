#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int


uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar yi,er,san,si,wu,liu,qi,ba;


void allinit();
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void delayms(uint ms)	;
void delay1ms()	;

//sbit CE=P1^3;
//sbit IO=P2^3;
//sbit SCLK=P1^7;
//#define OW_SKIP_ROM 0xcc
//#define DS18B20_CONVERT 0x44
//#define DS18B20_READ 0xbe

//IC引脚定义
//sbit DQ = P1^4;

//函数声明
//void Delay_OneWire(unsigned int t);
//void Write_DS18B20(unsigned char dat);
//bit Init_DS18B20(void);
//unsigned char Read_DS18B20(void);
//float tempget();

//int temp1;
//uchar ds[]={55,59,23,10,10,1,16};//放置初值 秒，分，时，日，月，星期，年     根据题目要求自己设置初值
//uchar shijian[7];
//void writebyte(uchar dat);
//void write(uchar add,uchar dat);
//void dsinit();
//uchar read(uchar add);
//uchar readbyte();
//void readtime();
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
void write_EEPROM(uchar add,uchar dat);
float AD_get(uchar add);
uint k;
void main()
{
	allinit();yi=0;er=0;san=0;si=10;
	while(1)
	{
//		temp1= tempget()*10;

//		yi=temp1/100;        //345
//		er=temp1/10%10;
//		san=temp1%10;
//		readtime();
//		yi=shijian[2]/10;er=shijian[2]%10;san=10;
//		si=shijian[1]/10;wu=shijian[1]%10;liu=10;
//		qi=shijian[0]/10;ba=shijian[0]%10;
		k=AD_get(0x03)*100;//4.3 430
		yi=k/100;
		er=k%100/10;
		san=k%10;
		display1(yi,er);
	  display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
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
float AD_get(uchar add)
{
	float temp;
	
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_Stop();
	
	temp=temp/255*5.00;
	return temp;
}
void write_EEPROM(uchar add,uchar dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();	

}
//void readtime()
//{
//	uchar i;
//	uchar add=0x81;
//	write(0x8e,0x00);
//	for(i=0;i<7;i++)
//	{
//		shijian[i]=read(add);
//		add=add+2;
//	}
//	write(0x8e,0x80);	
//}


//uchar read(uchar add)
//{
//	uchar i,temp;
//	uchar dat1,dat2;
//	CE=0;
//	SCLK=0;
//	CE=1;
//	writebyte(add);
//	for(i=0;i<8;i++)
//	{
//		SCLK=0;
//		temp>>=1;
//		if(IO)
//		{
//			temp|=0x80;
//		}
//		SCLK=1;
//	}
//	
//	IO=0;
//	
//	dat1=temp/16;//0XFF 1111 1111
//	dat2=temp%16;
//	
//	temp=dat1*10+dat2;
//	
//	return temp;
//}


//void writebyte(uchar dat)
//{
//	uchar i;
//	for(i=0;i<8;i++)
//	{
//		SCLK=0;
//		IO=dat&0x01;
//		SCLK=1;
//		dat>>=1;
//	}
//}

//void write(uchar add,uchar dat)
//{
//	uchar num;
//	
//	CE=0;
//	SCLK=0;
//	CE=1;
//	writebyte(add);
//	num=(dat/10<<4)|(dat%10);// 55
//	writebyte(num);
//	CE=0;
//}


//void dsinit()
//{
//	uchar i;
//	uchar add=0x80;
//	write(0x8e,0x00);
//	for(i=0;i<7;i++)
//	{
//		write(add,ds[i]);
//		add=add+2;
//	}
//	write(0x8e,0x80);
//	
//}
//单总线延时函数
//void Delay_OneWire(unsigned int t)
//{
//  while(t--);
//}

////DS18B20芯片初始化
//bit Init_DS18B20(void)
//{
//	bit initflag = 0;
//	DQ = 1;
//	Delay_OneWire(120);
//	DQ = 0;
//	Delay_OneWire(800); 
//	DQ = 1;
//	Delay_OneWire(100); 
//	initflag = DQ;    
//	Delay_OneWire(50);
//  
//	return initflag;
//}

////通过单总线向DS18B20写一个字节
//void Write_DS18B20(unsigned char dat)
//{
//	unsigned char i;
//	for(i=0;i<8;i++)
//	{
//		DQ = 0;
//		DQ = dat&0x01;
//		Delay_OneWire(50);
//		DQ = 1;
//		dat >>= 1;
//	}
//	Delay_OneWire(50);
//}

////从DS18B20读取一个字节
//unsigned char Read_DS18B20(void)
//{
//	unsigned char i;
//	unsigned char dat;
//  
//	for(i=0;i<8;i++)
//	{
//		DQ = 0;
//		dat >>= 1;
//		DQ = 1;
//		if(DQ)
//		{
//			dat |= 0x80;
//		}	    
//		Delay_OneWire(50);
//	}
//	return dat;
//}

//float tempget()
//{
//	uint temp;
//	float temp2;
//	uchar low,high;
//	Init_DS18B20();
//	Write_DS18B20(0xcc);
//	Write_DS18B20(0x44);
//	Delay_OneWire(500);
//	
//	Init_DS18B20();
//	Write_DS18B20(0xcc);
//	Write_DS18B20(0xbe);
//	Delay_OneWire(500);
//	low=Read_DS18B20();
//	high=Read_DS18B20();
//	
////	temp=high<<4;
////	temp|=(low>>4);

//    temp = high&0x0f;
//		temp = temp<<8|low;
//		temp2 = temp*0.0625;
//		return temp2;
//	
//}
void allinit()
{
	P2=0X80;
	P0=0XFF;
	
	P2=0XA0;
	P0=0X00;
	
	P2=0XC0;
	P0=0XFF;
	P2=0XFF;
	P0=0XFF;//数码管
}

void display1(uchar yi,uchar er)
{
  P2=0XC0;
	P0=0X01;
	P2=0XFF;
	P0=tab[yi]&0x7f;
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
	P0=tab[liu]&0X7F;
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

