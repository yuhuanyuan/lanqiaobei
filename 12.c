#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int



#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}    



//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */
uchar yi,er,san,si,wu,liu,qi,ba;	
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
	                 0Xc1,//u
	                 0XFF,
	                 0x11,//u
	                };

void IIC_Start(void);
void IIC_Stop(void);
bit IIC_WaitAck(void);//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt);
unsigned char IIC_RecByte(void);
unsigned long ad_read(uchar add);
									
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void delayms(unsigned int ms)	;
void delay1ms();
void allinit();
void keyscan();	
void Timer0Init(void);
									
unsigned long R3,R1;
uint pv3=200,pv1=200;	
uchar p=3;//通道
bit flag=0;//参数设计为1；显示界面为0									
void main()
{
	allinit();
	Timer0Init();
	while(1)
	{
		
//		R1=ad_read(0x01);
//		liu=R1/100;
//		qi=R1%100/10;
//		ba=R1%10;
//		R3=ad_read(0x03);
//		liu=R3/100;
//		qi=R3%100/10;
//		ba=R3%10;
		keyscan();
		R1=ad_read(0x01);R3=ad_read(0x03);
		if(flag==0)//显示界面
		{
			if(p==1)
			{
				
				yi=10;er=p;san=11;si=11;wu=11;liu=R1/100;qi=R1%100/10;ba=R1%10;
				
			}
			else if(p==3)
			{
				
				yi=10;er=p;san=11;si=11;wu=11;liu=R3/100;qi=R3%100/10;ba=R3%10;
			}
	  }
		else if(flag==1)//设置界面
		{
			if(p==1)
			{
			  
				yi=10;er=p;san=11;si=11;wu=11;liu=pv1/100;qi=pv1%100/10;ba=pv1%10;
				
			}
			else if(p==3)
			{
				
				yi=10;er=p;san=11;si=11;wu=11;liu=pv3/100;qi=pv3%100/10;ba=pv3%10;
			}
		}
		P2=0X80;P0=0XFF;
		if(R3>pv3){P2=0X80;P0&=0XFD;}//强行点亮l2
	else if(R3<pv3){P2=0X80;P0|=0X02;}
	if(R1>pv1){P2=0X80;P0&=0XFE;}//强行点亮l1
	else if(R3>pv3){P2=0X80;P0|=0X01;}
	if(p==1){P2=0X80;P0&=0XFB;}//强行点亮l3
	else if(p==3){P2=0X80;P0|=0X04;}
		display1(yi,er);
	  display2(san,si);
	  display3(wu,liu);
	  display4(qi,ba);
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
	EA=1;
	ET0=1;
}
void timer0() interrupt 1
{
//	if(R3>pv3){P2=0X80;P0&=0XFD;}//强行点亮l2
//	else if(R3<pv3){P2=0X80;P0|=0X02;}
//	if(R1>pv1){P2=0X80;P0&=0XFE;}//强行点亮l1
//	else if(R3>pv3){P2=0X80;P0|=0X01;}
//	if(p==1){P2=0X80;P0&=0XFB;}//强行点亮l3
//	else if(p==3){P2=0X80;P0|=0X04;}
	
}
void keyscan()
{
  if(P30==0)//s7加
	{
		delayms(5);
    if(P30==0)
		{
			if(flag==1)//参数设计
			{
				if(p==1){pv1+=20;}
				if(p==3){pv3+=20;}
				
			}
	
		}while(!P30);			
	}
 else if(P31==0)//s6减
	{
		delayms(5);
    if(P31==0)
		{
			if(flag==1)//参数设计
			{
				if(p==1){pv1-=20;}
				if(p==3){pv3-=20;}
				
			}
	
		}while(!P31);			
	}
 else if(P32==0)//s5界面转换
	{
		delayms(5);
    if(P32==0)
		{
			if(flag==1){flag=0;}
			else if(flag==0){flag=1;}
	
		}while(!P32);			
	}
 else if(P33==0)//s4通道选择
	{
		delayms(5);
    if(P33==0)
		{
			if(p==1)p=3;
			else if(p==3)p=1;
	
		}while(!P33);			
	}
}
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

unsigned long ad_read(uchar add)
{
	unsigned long temp;
	
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
	
  temp=temp*500/255;
	return temp;
}


