#include<STC15F2K60S2.H>
#include<intrins.h>


#define uchar unsigned char
#define uint unsigned int
	
//IC引脚定义
sbit DQ = P1^4;

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
									
void allinit();
void delayms(uint ms)	;
void display1(uchar yi,uchar er);		
void display2(uchar san,uchar si);	
void display3(uchar wu,uchar liu);	
void display4(uchar qi,uchar ba);										
void key();
void Timer0Init(void)		;

void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
bit Init_DS18B20(void);
unsigned char Read_DS18B20(void);
unsigned char tempget();
#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe
#define uchar unsigned char
#define uint unsigned int


								
void allinit()
{
	P2=0X80;P0=0XFF;
	P2=0XA0;P0=0X00;
	P2=0XC0;P0=0XFF;P2=0XFF;P0=0XFF;
}
void display1(uchar yi,uchar er)
{
	P2=0XC0;P0=0X01;P2=0XFF;P0=tab[yi];delayms(1);
	
	P2=0XC0;P0=0X02;P2=0XFF;P0=tab[er];delayms(1);
	
}
void display2(uchar san,uchar si)
{
	P2=0XC0;P0=0X04;P2=0XFF;P0=tab[san];delayms(1);
	
	P2=0XC0;P0=0X08;P2=0XFF;P0=tab[si];delayms(1);
	
}
void display3(uchar wu,uchar liu)
{
	P2=0XC0;P0=0X10;P2=0XFF;P0=tab[wu];delayms(1);
	
	P2=0XC0;P0=0X20;P2=0XFF;P0=tab[liu];delayms(1);	
}
void display4(uchar qi,uchar ba)
{
	P2=0XC0;P0=0X40;P2=0XFF;P0=tab[qi];delayms(1);
	
	P2=0XC0;P0=0X80;P2=0XFF;P0=tab[ba];delayms(1);		
}
void delayms(uint ms)	
{
  uint i,j;
	for(i=ms;i>0;i--)
	  for(j=845;j>0;j--);
}									
uint m;
uchar t,wendu,model=1,timec=0,PWM,shijian;	
bit flag;
void main()
{
	allinit();
	Timer0Init();
	yi=0;er=0;san=0;si=0;wu=0;liu=0;qi=0;ba=0;
	while(1)
	{
		key();
		wendu=tempget();

		if(model!=4)
		{
		  yi=10;er=model;san=10;si=11;wu=0;liu=timec/100;qi=shijian%100/10;ba=shijian%10;
		}
		else if(model==4)
		{
			yi=10;er=model;san=10;si=11;wu=0;liu=wendu%100/10;qi=wendu%10;ba=10;
		}
		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
		
	}
}

void key()
{
	if(P30==0)//s7
	{
		delayms(5);
		if(P30==0)
		{
			model=4;
		}while(!P30);
	}
	else if(P31==0)//s6停止
	{
		delayms(5);
		if(P31==0)
		{
			model=1;
			timec=0;
		}while(!P31);
	}
	else if(P32==0)//s5
	{
		delayms(5);
		if(P32==0)
		{
			
			if(timec==0)timec=60;
			else if(timec==60)timec=120;
			else if(timec==120)timec=0;
			shijian=timec;
		}while(!P32);
	}
	else if(P33==0)//s4
	{
		delayms(5);
		if(P33==0)
		{
			if(model==1){model=2;}
			else if(model==2){model=3;}
			else if(model==3){model=1;}
		}while(!P33);
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
	EA=1;ET0=1;
}


void timer0() interrupt 1
{
	
	t++;m++;
//	if(shijian>0)
//	{
//		if((model==1)&&(t==40))
//		{
//			
//			if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			if(t==200)
//			{
//				t=0;
//				if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			}
//		}
//		if((model==2)&&(t==80))
//		{
//			
//			if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			if(t==200)
//			{
//				t=0;
//				if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			}
//		}
//		if((model==3)&&(t==160))
//		{
//			
//			if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			if(t==200)
//			{
//				t=0;
//				if(flag==0){flag=1;P2=0X80;P01=0;}
//			else if(flag==1){flag=0;P2=0X80;P01=1;}
//			}
//		}
//  }	
	if(m==200)//1000000
	{
	  m=0;
		if(shijian>0)shijian--;
	}
}
void Delay_OneWire(unsigned int t)
{
  while(t--);
}

//DS18B20芯片初始化
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(120);
	DQ = 0;
	Delay_OneWire(800); 
	DQ = 1;
	Delay_OneWire(100); 
	initflag = DQ;    
	Delay_OneWire(50);
  
	return initflag;
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(50);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(50);
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
		Delay_OneWire(50);
	}
	return dat;
}
unsigned char tempget()
{
	uchar low,high,temp;
	
	Init_DS18B20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0X44);
	delayms(1);
	
	Init_DS18B20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);

	low=Read_DS18B20();
	high=Read_DS18B20();
	
	temp=high<<4;
	temp|=(low>>4);
	
	return temp;
}