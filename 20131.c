#include<STC15F2K60S2.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int


void allinit();
void key34();
void delay_ms(unsigned int ms);
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);

uchar code tab[]={ 0xc0,//0
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
int key,tt;									
bit set=0,clear=0,flg=0,flag=0;									
uchar wendu,model,max=23,min=22;
									


#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

sbit DQ = P1^4;

//函数声明
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
bit Init_DS18B20(void);
unsigned char Read_DS18B20(void);
uchar DSread();		
void Timer0Init(void)	;		
							
void main()
{
	allinit();Timer0Init();
	while(1)
	{


		if(set==0)//wendu display
		{
			wendu=DSread();
			if(wendu>max)model=2;
			else if(wendu<min)	model=0;
			else if((wendu>min)&&(wendu<max))model=1;
		  yi=10;er=model;san=10;si=11;wu=11;liu=10;qi=wendu/10;ba=wendu%10;
		}
		
		else if(set==1)//wendu set
		{
			
			if(clear==1)
			{
				clear=0;
				yi=10;er=11;san=11;si=11;wu=11;liu=10;qi=11;ba=11;
			}
		
			if((er==11)&&(flag==1)){flag=0;er=key;}
			else if((san==11)&&(flag==1)){flag=0;san=key;}
			else if((qi==11)&&(flag==1)){flag=0;qi=key;}
			else if((ba==11)&&(flag==1))
       {
				 
			   flag=0;
			   ba=key;
				 
				 if((er*10+san)<(qi*10+ba))
				 {P2=0X80;P0=0XFD;}
				 else
				 {
					 max=er*10+san;
					 min=qi*10+ba;
					 P2=0X80;P0=0XFF;
					
				 }
			 }
		}
		key34();
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
	tt++;
	if((model==0)&&(tt==160))
	{
		tt=0;P2=0XA0;P0=0X00;
		
    if(flg==0){flg=1;P2=0x80;P0=0XEF;}
		else if(flg==1){flg=0;P2=0x80;P0=0XFF;} 
	
	}
	else if((model==1)&&(tt==80))
	{
		tt=0;P2=0XA0;P0=0X00;
		
    if(flg==0){flg=1;P2=0x80;P0=0XFE;}
		else if(flg==1){flg=0;P2=0x80;P0=0XFF;} 
	
	}
		else if((model==2)&&(tt==40))
	{
		tt=0;P2=0XA0;P0=0X10;
		
    if(flg==0){flg=1;P2=0x80;P0=0XFD;}
		else if(flg==1){flg=0;P2=0x80;P0=0XFF;} 
	
	}
}
void allinit()
{
	P2=0X80;P0=0XFF;//LED
	P2=0XA0;P0=0X00;//BEE RELAY
	P2=0XC0;P0=0XFF;P2=0XFF;P0=0XFF;//数码管
}
void delay_ms(unsigned int ms)
{
	  unsigned int i,j;
	for(i=ms;i>0;i--)
	  for(j=845;j>0;j--);

}
void display1(uchar yi,uchar er)
{
	P2=0XC0;P0=0X01;
	P2=0XFF;P0=tab[yi];delay_ms(1);
	
	P2=0XC0;P0=0X02;
	P2=0XFF;P0=tab[er];delay_ms(1);

}	
void display2(uchar san,uchar si)
{
	P2=0XC0;P0=0X04;
	P2=0XFF;P0=tab[san];delay_ms(1);
	
	P2=0XC0;P0=0X08;
	P2=0XFF;P0=tab[si];delay_ms(1);

}
void display3(uchar wu,uchar liu)
{
	P2=0XC0;P0=0X10;
	P2=0XFF;P0=tab[wu];delay_ms(1);
	
	P2=0XC0;P0=0X20;
	P2=0XFF;P0=tab[liu];delay_ms(1);

}
void display4(uchar qi,uchar ba)
{
	P2=0XC0;P0=0X40;
	P2=0XFF;P0=tab[qi];delay_ms(1);
	
	P2=0XC0;P0=0X80;
	P2=0XFF;P0=tab[ba];delay_ms(1);

}
void key34()
{
	uchar temp;
	P42=1;P44=0;P3=0X7F;
	temp=P3;temp=temp&0X0F;
	if(temp!=0X0F)
	{
		delay_ms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0X7E:key=0;flag=1;break;
				case 0X7D:key=3;flag=1;break;
				case 0X7B:key=6;flag=1;break;
				case 0X77:key=9;flag=1;break;
			}while(temp!=0x0f)
			{
			  temp=P3;temp=temp&0X0F;
			}
				
		
		}

	}
	
	P42=0;P44=1;P3=0XBF;
	temp=P3;temp=temp&0X0F;
	if(temp!=0X0F)
	{
		delay_ms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0XBE:key=1;flag=1;break;
				case 0XBD:key=4;flag=1;break;
				case 0XBB:key=7;flag=1;break;
				case 0XB7:
					if(set==0)
					{
						set=1;P2=0X80;P0=0XFF;P2=0XA0;P0=0X00;
						yi=10;er=11;san=11;si=11;wu=11;liu=10;qi=11;ba=11;
					
					}	
					else if(set==1)
					{
						set=0;P2=0X80;P0=0XFF;P2=0XA0;P0=0X00;
						
					}
					break;
			}while(temp!=0x0f)
			{
			  temp=P3;temp=temp&0X0F;
			}
				
		
		}

	}
	
	
		P42=1;P44=1;P3=0XDF;
	temp=P3;temp=temp&0X0F;
	if(temp!=0X0F)
	{
		delay_ms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0XDE:key=2;flag=1;break;
				case 0XDD:key=5;flag=1;break;
				case 0XDB:key=8;flag=1;break;
				case 0XD7:clear=1;break;
			}while(temp!=0x0F)
			{
			  temp=P3;temp=temp&0X0F;
			}
				
		
		}

	}
	


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



uchar DSread()
{
	uchar low,high,temp;
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	delay_ms(1);
	
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	
	low=Read_DS18B20();
	high=Read_DS18B20();
	
	temp=high<<4;
	temp|=(low>>4);
	delay_ms(1);
	
	return temp;
}