#include<STC15F2K60S2.H>
#include<intrins.h>
#include<iic.h>
#include<ds1302.h>

#define uchar unsigned char
#define uint unsigned int

sbit SDA = P2^1;  
sbit SCL = P2^0;  
bit statue=0,bee,relay,s6=0;
bit jia=0;
bit jian=0;
uchar fuzhi;

uchar code tab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf,0xff};
uchar yi,er,san,si,wu,liu,qi,ba;
extern unsigned char shijian[]={0,30,8,0,0,0,0};

void allinit();
void Delay1ms();
void Delay5ms();
void keyscan();
void somenop();
unsigned char IIC_RecByte(void);
void IIC_SendByte(unsigned char byt);


void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);

uchar shidu;

void main()
{

  allinit();

	yi=11;er=0;san=5;si=0;wu=0;liu=1;qi=0;ba=0;
	EEPROM_write(0x10,50);
	Delay5ms();
	fuzhi=EEPROM_read(0x10);
  qi=fuzhi/10;ba=fuzhi%10;

	while(1)
	{

//		shidu= AD_read(0x03)*0.39;
//		liu=shidu/100;qi=shidu%100/10;ba=shidu%10;
//		EEPROM_write(0x10,50);
//		Delay5ms();
//		fuzhi=EEPROM_read(0x10);
//		liu=shidu/100;qi=shidu%100/10;ba=shidu%10;
		
		
		get_ds1302();
		keyscan();
		shidu=AD_read(0x03)*0.39;
	  P2=0X80;P0=0XFE;
		if(statue==0)
		{
								if(shidu>fuzhi)
								{
									 P2=0xa0;P0=0x00;
								}
								else
								{
									 P2=0XA0;P0=0X10;
								}
								if(s6==1)
								{
									if(jia==1){jia=0;fuzhi=fuzhi+1;}
									if(jian==1){jian=0;fuzhi=fuzhi-1;}
									yi=10;er=10;san=11;si=11;wu=11;liu=11;qi=fuzhi/10;ba=fuzhi%10;
								}
								else if (s6==0)
								{
									yi=shijian[2]/10;er=shijian[2]%10;san=10;
									si=shijian[1]/10;wu=shijian[1]%10;liu=11;
									qi=shidu/10;ba=shidu%10;
								
								}
							 display1(yi,er);
							 display2(san,si);
							 display3(wu,liu);
							 display4(qi,ba);
		}
		
		else if(statue==1)
		{
		
		  if(shidu<fuzhi)
				bee=1;
		
		}


	}

}
void keyscan()
{
  if(P30==0)//s7
	{
		Delay5ms();
	   if(P30==0)
		 {
				 if(statue==0)
				 {statue=1;P2=0X80;P0=0XFE;}
				 else if(statue==1)
				 {statue=0;P2=0X80;P0=0XFD;}
		 }while(!P30);
	 
	}
   if(P31==0)//s6
		{
				Delay5ms();
				if(P31==0)
				{	
						   if(statue==0)
							{
										  if(s6==0)
										 {
											s6=1;//进入调整状态
										 }
											if(s6==1)
										 {
											s6=0;
										  EEPROM_write(0x10,fuzhi); //进入存储EEPROM
										 }
									 
						 }
						 else if(statue==1)	
						{
				   		 if((shidu<fuzhi)&&(bee==1))
											bee=0;
									
						}
				}while(!P31);			
								
		}
		
	   if(P32==0)//s5
	{
			Delay5ms();
	    if(P32==0)
				 {
					  if(statue==0)
					 {
					   jia=1;
				   }
					 else if(statue==1)
					 {
					   relay=1;
					 }
			   }while(!P32);
	 
	}
	   if(P33==0)
	{  
			Delay5ms();
	   if(P33==0)
		 {
			 if(statue==0)
			 {
        jian=1;
		   }
			 else if(statue==1)
			 {
			   relay=0;
			 }
	 }while(!P33);
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