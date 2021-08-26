
#include<stc15f2k60s2.h>
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int


 
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1
 
//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */
 
//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
 
unsigned char AD_Read(unsigned char add);
unsigned char EEPROM_Read(unsigned char add);
void EEPROM_Write(unsigned char dat,unsigned char add);
uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
 
uchar start=0;
uchar moshi=1;
uchar zhuanhua=0;
uint jiange=400;
uint led_t=0;
uint zhuanhua_t=0;
uint ad_t=0;
uint RB2=0;
uchar flag=0;
uchar led_n=0;
uchar led_TT=0,light=0;
uchar f1,f2,f3,f4,f5,f6,f7,f8;
 
void allinit();
void delayms(uchar ms);
void keyscan();
void Time0_init();
void display12(uchar f1,uchar f2);
void display34(uchar f3,uchar f4);
void display56(uchar f5,uchar f6);
void display78(uchar f7,uchar f8);
 
void main(){
allinit();
Time0_init();
moshi=EEPROM_Read(0x10);
delayms(5);
jiange=EEPROM_Read(0x20)*100;
delayms(5);
f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=11;f8=11;
while(1){
    if((zhuanhua==0)&&(flag==1))
	{
	   if(RB2<64)
	   {
	     f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=10;f8=1;
	   }
	   else if((RB2>=64)&&(RB2<128))
	   {
	     f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=10;f8=2;
	   }
	   else if((RB2>=128)&&(RB2<192))
	   {
	     f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=10;f8=3; 
	   }
	   else if((RB2>=192)&&(RB2<255))
	   {
	     f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=10;f8=4;
	   }
	}
	else if(zhuanhua==0)
	 {
	   f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=11;f8=11;
	 }
	 keyscan();
	 display12(f1,f2);
	 display34(f3,f4);
	 display56(f5,f6);
	 display78(f7,f8);
}
 
}
void keyscan(){
if(P30==0)
{
  delayms(5);
  if(P30==0)
  {
    if(start==0)
	{
	  start=1;
	}
	else if(start==1)
	{
	  start=0;
	}
}
while(!P30);
}
 
else if(P31==0)
{
  delayms(5);
  if(P31==0)
  {
    if(zhuanhua==0)
	{
	  zhuanhua=1;zhuanhua_t=0;
	  if(jiange>=1000)
	  {
	    f1=10;f2=moshi;f3=10;f4=11;f5=jiange/1000;f6=jiange%1000/100;f7=jiange%100/10;f8=jiange%10;
	  }
	  else
	  {
	    f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=jiange/100;f7=jiange%100/10;f8=jiange%10;
	  }
	}
	else if(zhuanhua==1)
	{
	  zhuanhua=2;zhuanhua_t=0;
	  if(jiange>=1000)
	  {
	    f1=10;f2=moshi;f3=10;f4=11;f5=jiange/1000;f6=jiange%1000/100;f7=jiange%100/10;f8=jiange%10;
	  }
	  else
	  {
	    f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=jiange/100;f7=jiange%100/10;f8=jiange%10;
	  }
	}
	else if(zhuanhua==2)
	{
	  zhuanhua=0;zhuanhua_t=0;
	  f1=11;f2=11;f3=11;f4=11;f5=11;f6=11;f7=11;f8=11;
	  EEPROM_Write(moshi,0x10);
	  delayms(5);
	  EEPROM_Write(jiange/100,0x20);
	  delayms(5);
	}
}
while(!P31);
}
 
else if(P32==0)
{
  delayms(5);
  if(P32==0)
  {
     if(zhuanhua==1)
	 {
	   moshi+=1;
	   if(moshi==5)
	   {
	     moshi=1;
	   }
	 }
	 else if(zhuanhua==2)
	 {
	   jiange+=100;
	   if(jiange==1300)
	   {
	     jiange=400;
	   }
	 }
   }
while(!P32);
}
 
else if(P33==0)
{
  delayms(5);
  if(P33==0)
  {
    flag=1;
  }
}
 
if((P33==1)&&(flag==1))
{
  flag=0;
  if(zhuanhua==1)
  {
    moshi-=1;
	if(moshi==0)
	{
	  moshi=4;
	}
  }
  else if(zhuanhua==2)
  {
    jiange-=100;
	if(jiange==300)
	{
	  jiange=1200;
	}
  }
}
}
 
void Time0_init(){
TMOD=0X01;
TH0=(65536-1000)/256;
TL0=(65536-1000)%256;
TR0=1;
EA=1;
ET0=1;
}
 
void Time0_service() interrupt 1
{
 TH0=(65536-1000)/256;
 TL0=(65536-1000)%256;
 zhuanhua_t++;
 led_t++;
 ad_t++;
 if((led_t==1)&&(start==1))
 {
   if(moshi==1)
   {
    P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
	P0=~(0X01<<led_n);
   }
   else if(moshi==2)
   {
    P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
	P0=~(0X80>>led_n);
   }
   else if(moshi==3)
   {
     P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
	 if(led_n==0) P0=0X7E;
	 else if(led_n==1) P0=0XBD;
	 else if(led_n==2) P0=0XDB;
	 else if(led_n==3) P0=0XE7;
   }
   else if(moshi==4)
   {
     P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
	 if(led_n==0) P0=0XE7;
	 else if(led_n==1) P0=0XDB;
	 else if(led_n==2) P0=0XBD;
	 else if(led_n==3) P0=0X7E;
   }
 }
 else if(led_t==light)
 {
   	P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
 }
 else if(led_t==20)
 {
    led_t=0;led_TT++;
	if(20*led_TT>=jiange)
	{
	  led_TT=0;
	  if(moshi==1)
	  {
	     led_n++;
		 if(led_n>=8)
		 {
		   led_n=0;
		 }
	  }
	  else if(moshi==2)
	  {
	     led_n++;
		 if(led_n>=8)
		 {
		   led_n=0;
		 }
	  }
	  else if(moshi==3)
	  {
	     led_n++;
		 if(led_n>=4)
		 {
		   led_n=0;
		 }
	  }
	  else if(moshi==4)
	  {
	    led_n++;
		if(led_n>=4)
		{
		  led_n=0;
		}
	  }
	}
  }
  if(ad_t==200)
  {
    ad_t=0;
	RB2=AD_Read(0x03);
	if(RB2<64) light=2;
	else if((RB2>=64)&&(RB2<128)) light=5;
	else if((RB2>=128)&&(RB2<192)) light=10;
	else if((RB2>=192)&&(RB2<255)) light=19;
  }
 
  if(zhuanhua_t==800)
  {
     if(zhuanhua==1)
	 {
	   if(jiange>=1000)
	   {
	      f1=11;f2=11;f3=11;f4=11;f5=jiange/1000;f6=jiange%1000/100;f7=jiange%100/10;f8=jiange%10;
	   }
	   else
	   {
	      f1=11;f2=11;f3=11;f4=11;f5=11;f6=jiange/100;f7=jiange%100/10;f8=jiange%10;
	   }
	 }
	 else if(zhuanhua==2)
	 {
	   if(jiange>=1000)
	   {
	      f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=11;f7=11;f8=11;
	   }
	   else
	   {
	      f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=11;f7=11;f8=11;
	   }
	 }
  }
 
  if(zhuanhua_t==1600)
  {
    zhuanhua_t=0;
	if(zhuanhua==1)
	{
	   if(jiange>=1000)
	   {
	     f1=10;f2=moshi;f3=10;f4=11;f5=jiange/1000;f6=jiange%1000/100;f7=jiange%100/10;f8=jiange%10;
	   }
	   else
	   {
	      f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=jiange/100;f7=jiange%100/10;f8=jiange%10;
	   }
	}
	else if(zhuanhua==2)
	{
	   if(jiange>=1000)
	   {
	     f1=10;f2=moshi;f3=10;f4=11;f5=jiange/1000;f6=jiange%1000/100;f7=jiange%100/10;f8=jiange%10;
	   }
	   else
	   {
	      f1=10;f2=moshi;f3=10;f4=11;f5=11;f6=jiange/100;f7=jiange%100/10;f8=jiange%10;
	   }
	} 
 }
} 
 
void allinit(){
P2=0XA0;P0=0X00;P2=0X80;P0=0XFF;
P2=0XC0;P0=0XFF;P2=0XFF;P0=0XFF;
}
 
void delayms(uchar ms){
uchar i,j,k;
for(k=ms;k>0;k--){
   i=12;
   j=169;
   do
   {
      while(j--);
   }
   while(i--);
 }
}
 
void display12(uchar f1,uchar f2){
P2=0XC0;P0=0X01;P2=0XFF;P0=tab[f1];
delayms(1);
P2=0XC0;P0=0X02;P2=0XFF;P0=tab[f2];
delayms(1);
}
 
void display34(uchar f3,uchar f4){
P2=0XC0;P0=0X04;P2=0XFF;P0=tab[f3];
delayms(1);
P2=0XC0;P0=0X08;P2=0XFF;P0=tab[f4];
delayms(1);
}
 
void display56(uchar f5,uchar f6){
P2=0XC0;P0=0X10;P2=0XFF;P0=tab[f5];
delayms(1);
P2=0XC0;P0=0X20;P2=0XFF;P0=tab[f6];
delayms(1);
}
 
void display78(uchar f7,uchar f8){
P2=0XC0;P0=0X40;P2=0XFF;P0=tab[f7];
delayms(1);
P2=0XC0;P0=0X80;P2=0XFF;P0=tab[f8];
delayms(1);
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
 
unsigned char AD_Read(unsigned char add)
{
		unsigned char temp;
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
		return temp;
}
 
//EEPROM读
unsigned char EEPROM_Read(unsigned char add)
{
		unsigned char temp;
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
 
 
//EEPROMD写
void EEPROM_Write(unsigned char dat,unsigned char add)
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



