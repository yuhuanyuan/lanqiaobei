#include<STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
	#define uint unsigned int 
unsigned int shijian[]={0,30,8,11,3,0,21};//s,min,hour,date,month,day,year
uchar yi,er,san,si,wu,liu,qi,ba,temp,yue,nian,ri;
bit s7,s6,s5,s4,jia,jian;
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


sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302¸´Î»		
									
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

								

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
uchar AD_read(uchar add);

void delay1ms()	;
void DS_init();	
void DS_get();
void keyscan();		
void delayms(uint ms)	;									
									
void main()
{
 allinit();
	DS_init();
	yi=2;er=0;san=0;si=0;wu=0;liu=1;qi=0;ba=1;		yue=1;nian=00;ri=1;
	while(1)
	{
		
		 DS_get();
		 keyscan();

		 shijian[4]=yue;shijian[6]=nian;shijian[3]=ri;
		 if(s7==1)
		 {
			 yi=shijian[2]/10;er=shijian[2]%10;san=tab[11];
			 si=shijian[1]/10;wu=shijian[1]%10;liu=tab[11];
			 qi=shijian[0]/10;ba=shijian[0]%10;
			 

		 }
		 else if(s7==0)
		 {
			 
			 yi=2;er=0;san=shijian[6]/10;si=shijian[6]%10;
			 wu=shijian[4]/10;liu=shijian[4]%10;
			 qi=shijian[3]/10;ba=shijian[3]%10;
		 }
   
//	   
		

		 display1(yi,er);
     display2(san,si);
     display3(wu,liu);
     display4(qi,ba);
	}

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
		  if(s7==0) s7=1;
		  else if(s7==1) s7=0;
		}while(!P30);			
	}
  else if(P31==0)
	{
		delayms(5);
    if(P31==0)
		{
		  nian=nian+1;
		}while(!P31);			
	}
  else if(P32==0)
	{
		delayms(5);
    if(P32==0)
		{
		 yue=yue+1;
		}while(!P32);			
	}
  else if(P33==0)
	{
		delayms(5);
    if(P33==0)
		{
		  ri=ri+1;
		}while(!P33);			
	}


}

void allinit()
{
  P2=0X80;P0=0XFF;//LED
	P2=0XA0;P0=0X00;//BEE
	P2=0XC0;P0=0XFF;P2=0XFF;P0=0XFF;//ÊýÂë¹Ü

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




