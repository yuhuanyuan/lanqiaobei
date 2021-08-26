#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int
	
//P34 T0 计数器
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
	                 
	                 0XFF,
	                 0x8e,//u
	                };
unsigned long int fval;
uchar t_flag;		
void Timer1Init();
void	Timer0Init();
									
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void delayms(unsigned int ms)	;
void delay1ms();
void allinit();
									
void main()
{
	allinit();
	Timer1Init();Timer0Init();
	while(1)
	{
		
			yi=11;					//显示“F”
	er=10;					//不显示
	san=fval/100000%10;	//显示频率值
	si=fval/10000%10;
	wu=fval/1000%10;
	liu=fval/100%10;
	qi=fval/10%10;
	ba=fval%10;

		
		display1(yi,er);
	  display2(san,si);
	  display3(wu,liu);
	  display4(qi,ba);
	}
}


void Timer1Init()		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	EA=1;
	ET1=1;
}

void fread() interrupt 3 using 1
{
	if(++t_flag>=10){
		t_flag=0;
		fval=TH0;		//记录计数器的值
		fval<<=8;
		fval|=TL0;
		fval<<=1;		//相当于fval*=2;
		TH0=0x00;
		TL0=0x00;
		TF1=0;
	}		
}
void Timer0Init()
{
	AUXR &= 0x7F;					//定时器时钟1T模式
    TMOD = 0x04;          //设置定时器0为16位自动重装载外部记数模式
    TH0 = TL0 = 0;     		//设置定时器0初始值
	TR0 = 1;              //定时器0开始工作
	EA=1;
	ET0=1;
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


