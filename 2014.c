
/*存在s7按键按下后不能初始的问题*/

#include<STC15F2K60S2.H>
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
									
void allinit();
void delayms(uint ms)	;
void display1(uchar yi,uchar er);		
void display2(uchar san,uchar si);	
void display3(uchar wu,uchar liu);	
void display4(uchar qi,uchar ba);										
void key();



uchar shijian[]={50,59,23,0,0,0,0};//s,m,h

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位		
									
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

void Write_Ds1302_Byte(unsigned  char temp) ;
void DS_init();
void DS_get();
void Write_Ds1302( unsigned char address,unsigned char dat )  ;   
unsigned char Read_Ds1302 ( unsigned char address) ;


/*wendu*/
#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

//IC引脚定义
sbit DQ = P1^4;

//函数声明
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
bit Init_DS18B20(void);
unsigned char Read_DS18B20(void);
uchar tempget();
  uchar i;

uchar wendu,model=1,flag=0,time=1,t,suoyin=0,cai;
bit g=0,f=0,led=0;
uchar wendu1[10];
void Timer0Init(void);

void main()
{
	allinit();
	DS_init();
	Timer0Init();
	yi=11;er=11;san=11;si=11;wu=11;liu=10;qi=0;ba=1;
	while(1)
	{
  
		key();
		wendu=tempget();
	
				if(cai==1)//时间间隔确定，显示时间msh
		{
				DS_get();
			
				if(shijian[0]%2==1){yi=shijian[2]/10;er=shijian[2]%10;san=11;si=shijian[1]/10;wu=shijian[1]%10;liu=11;ba=shijian[0]%10;qi=shijian[0]/10;}
				else if(shijian[0]%2==0){yi=shijian[2]/10;er=shijian[2]%10;san=10;si=shijian[1]/10;wu=shijian[1]%10;liu=10;ba=shijian[0]%10;qi=shijian[0]/10;}
	
		
		}
	
         if(cai==7)
				 {
			
					 yi=11;er=11;san=11;si=11;wu=11;liu=10;qi=0;ba=1;
					 model=1;
					 time=1;
					 suoyin=0;
				 }


		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
			
		}

//		wendu=tempget();
//		DS_get();
//		yi=shijian[0]/10;er=shijian[0]%10;san=shijian[1]/10;si=shijian[1]%10;
//		qi=wendu/10;ba=wendu%10;

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
void timer0() interrupt 1
{

	t++;
	if(cai==1)//时间间隔确定，开始采集
	{
				if(t==time*200)//存储
				{
					 t=0;
					 wendu1[suoyin]=wendu;
					 suoyin++;
								if(suoyin==10)
							{
								cai=6;led=1;
								yi=10;er=0;san=0;si=11;wu=11;liu=10;qi=wendu1[0]/10;ba=wendu1[0]%10;
							}

					
				}
		
	}
	
	if((led==1)&&(t=100))
	{
		t=0;
		if(g==0){g=1;P2=0X80;P0=0XFF;}
		else if(g==1){g=0;P2=0X80;P0=0X00;}
	}
	
	if(cai==2)//按下s6进行显示
		{
				if(t==150)
				{
					t=0;led=0;
					yi=10;er=10;san=i;si=11;wu=11;liu=10;qi=wendu1[i]/10;ba=wendu1[i]%10;i++;
					if(i==10)i=0;
				}
				
		 }
		

}
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
void key()
{
	if(P30==0)//s7
	{
		delayms(5);
	  if(P30==0)
		{
			EA=0;ET0=0;
			cai=7;
			
			

		}while(!P30);
	
	}
		if(P31==0)//s6
	{
		delayms(5);
	  if(P31==0)
		{
			led=0;
			EA=1;ET0=1;cai=2;//显示
			P2=0X80;P0=0XFF;//关灯
		}while(!P31);
	
	}
		if(P32==0)//s5
	{
		delayms(5);
	  if(P32==0)
		{
			time=model;
      EA=1;ET0=1;cai=1;//存储


		}while(!P32);
	
	}
		if(P33==0)//s4
	{
		delayms(5);
	  if(P33==0)
		{
			 EA=0;ET0=0;cai=5;
       if(flag==0){flag=1;model=1;}
       else if(flag==1){flag=2;model=2;}			
       else if(flag==2){flag=3;model=3;}		
       else if(flag==3){flag=0;model=4;}		
       	yi=11;er=11;san=11;si=11;wu=11;liu=10;qi=model/10;ba=model%10; 
		}while(!P33);
	
	}
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



//单总线延时函数
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

uchar tempget()
{
	uchar temp;
	uchar high,low;
	
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	delayms(1);

	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);	
	
	low=Read_DS18B20();
	high=Read_DS18B20();
	
	temp=high<<4;
	temp|=(low>>4);
	
	return temp;
}