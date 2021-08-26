#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char 
#define uint unsigned int	
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

sbit DQ=P1^4;
sbit scl=P2^0;//IIC
sbit sda=P2^1;//IIC
sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位		
bit s7;								

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1


uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar yi,er,san,si,wu ,liu,qi,ba,wendu,cunchu,miao;
unsigned int shijian[]={0,0,0,0,0,0,0};//s,min,hour,date,month,day,year

uchar Get_DS18B20();
bit Init_DS18B20(void);
void dsinit();
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);

void allinit();
void keyscan();
void delayms(int ms);
void Delay100us()	;
void Delay5ms()	;
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void EEP_write(uchar add,uchar dat);
uchar EEP_read(uchar add);
bit IIC_WaitAck(void);
void IIC_Stop(void);
void IIC_Start(void);
void IIC_SendByte(unsigned char byt);
unsigned char IIC_RecByte(void);

void DS_get();

void main()
{
allinit();
yi=11;er=11;san=11;si=11;wu=10;liu=0;qi=0;ba=0;
while(1)
{
	DS_get();
	keyscan();
	miao=shijian[0];
	if((miao%10)==3)
	{
	  EEP_write(0X04,wendu);
    Delay5ms();
    cunchu=EEP_read(0X04);
	}
	if((miao%10)==0)
	{
	  wendu=Get_DS18B20();
	}

	
	liu=0;
	qi=wendu%100/10;
	ba=wendu%10;
	
	si=shijian[0]/10;
	wu=shijian[0]%10;
	if(s7==0)
	{
		yi=cunchu/10;
		er=cunchu%10;
		san=10;
	}
	else if(s7==1)
	{
	  yi=11;er=11;san=10;
	}
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
		delayms(5);
    if(P30==0)
		{
		  if(s7==0) s7=1;
		  else if(s7==1) s7=0;
		}while(!P30);			
	}

}

void allinit()
{
P2=0X80;P0=0XFF;
P2=0XA0;P0=0X00;

P2=0XC0;P0=0XFF;
P2=0XFF;P0=0XFF;
}
void delayms(int ms)
{
int i,j;
for(i=ms;i>0;i--)
	for(j=845;j>0;j--);
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









//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
unsigned char i;
for(i=0;i<8;i++)
{
	DQ = 0;
	DQ = dat&0x01;
  Delay100us();
	DQ = 1;
	dat >>= 1;
}
Delay100us()	;
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
unsigned char i;
unsigned char dat;

for(i=0;i<8;i++)
{
	DQ = 0;
	dat >>= 1;
	DQ = 1;
	if(DQ)
	{
		dat |= 0x80;
	}	    
	Delay100us();
}
return dat;
}
void Delay100us()		//@11.0592MHz
{
unsigned char i, j;

_nop_();
_nop_();
i = 2;
j = 15;
do
{
	while (--j);
} while (--i);
}


void dsinit()
{
DQ=0;
Delay100us();Delay100us();Delay100us();Delay100us();Delay100us();
DQ=1;
Delay100us();Delay100us();Delay100us();Delay100us();Delay100us();
}
uchar Get_DS18B20()
{
  uchar temp,low,high;

  dsinit();
  Write_DS18B20(0XCC);
  Write_DS18B20(0X44);
  delayms(1);
  dsinit();
  Write_DS18B20(0XCC);
  Write_DS18B20(0XBE);

  low=Read_DS18B20();
  high=Read_DS18B20();
  temp=high<<4;
  temp|=(low>>4);

  return temp;
}

void IIC_Start(void)
{
	sda = 1;
	somenop;
	scl = 1;
	somenop;
	sda = 0;
	somenop;
	scl= 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	sda = 0;
	somenop;
	scl = 1;
	somenop;
	sda = 1;
	somenop;
}

//等待应答
bit IIC_WaitAck(void)
{
	sda = 1;
	somenop;
	scl = 1;
	somenop;
	if(sda)    
	{   
		scl = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		scl = 0;
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
			sda = 1;
		}
		else 
		{
			sda = 0;
		}
		somenop;
		scl = 1;
		byt <<= 1;
		somenop;
		scl = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		scl = 1;
		somenop;
		da <<= 1;
		if(sda) 
		da |= 0x01;
		scl = 0;
		somenop;
	}
	return da;
}


uchar EEP_read(uchar add)
{
	uchar temp;
	
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_Stop();
	
	return temp;
}

void EEP_write(uchar add,uchar dat)
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

void Write_Ds1302_Byte(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;	
   	_nop_();  
 	Write_Ds1302_Byte(address);	
 	Write_Ds1302_Byte((dat/10<<4)|(dat%10));		
 	RST=0; 
}

unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00,dat1,dat2;
 	RST=0;
	_nop_();
 	SCK=0;
	_nop_();
 	RST=1;
	_nop_();
 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SDA=0;
	_nop_();
	SDA=1;
	_nop_();
	
	dat1=temp/16;dat2=temp%16;
	temp=dat1*10+dat2;
	return (temp);			
}
void DS_init()
{
   uchar i,add;
   add=0x80;
   Write_Ds1302(0x8e,0x00);
    for(i=0;i<7;i++)
  {
       Write_Ds1302(add,shijian[i]);
        add=add+2; 

  }
 Write_Ds1302(0x8e,0x80);
}

void DS_get()
{
   uchar i,dat;
   dat=0x81;
   Write_Ds1302(0x8e,0x00);
    for(i=0;i<7;i++)
  {
       shijian[i]=Read_Ds1302(dat);
        dat=dat+2; 

  }
 Write_Ds1302(0x8e,0x80);

}




