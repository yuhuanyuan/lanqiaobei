#include<STC15F2K60S2.H>
#include<intrins.h>

//IC引脚定义
sbit DQ = P1^4;

#define uchar unsigned char
#define uint unsigned int
sbit CE=P1^3;
sbit IO=P2^3;
sbit SCLK=P1^7;



uchar  ds[]={50,59,23,0,0,0,0};//s,m,h
uchar naozhong[]={0,0,0};//s,m,h
uchar shijian[7];



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
void Timer0Init(void);

void Delay_OneWire(unsigned int t);
bit Init_DS18B20(void);
void Write_DS18B20(unsigned char dat);
unsigned char Read_DS18B20(void);
uchar tempget();
									
void ds_init();		
void ds_get();
void writebyte(uchar dat);
void write(uchar add,uchar dat);
uchar read(uchar add);
uchar readbyte();
void Timer0Init(void);		//5毫秒@11.0592MHz

									
									
bit flag,shizhong,nao,jia,jian;									
uchar wendu,t,k=1,q=1,change;
uint m;
void main()
{
	allinit();
	ds_init();Timer0Init();
	yi=0;er=0;san=0;si=0;wu=0;liu=0;qi=0;ba=0;
	while(1)
	{
		key();	ds_get();
//		wendu=tempget();
//		yi=0;er=0;san=0;si=0;wu=0;liu=10;qi=wendu/10;ba=wendu%10;
		
//		yi=shijian[2]/10;er=shijian[2]%10;san=10;si=shijian[1]/10;wu=shijian[1]%10;liu=10;qi=shijian[0]/10;ba=shijian[0]%10;
	  if(shizhong==nao)	
		{	yi=shijian[2]/10;er=shijian[2]%10;san=10;si=shijian[1]/10;wu=shijian[1]%10;liu=10;qi=shijian[0]/10;ba=shijian[0]%10;}

		
		if(shizhong==1)
		{
			if(k==1)
			{
			  
				yi=shijian[2]/10;er=shijian[2]%10;san=10;si=shijian[1]/10;wu=shijian[1]%10;liu=10;qi=shijian[0]/10;ba=shijian[0]%10;

			}
			else if(k==2)
			{
				
				if(shijian[0]%2==0)
				{
					if(jia==1){jia=0;change=ds[2];change=change+1;ds[2]=change;}
					if(jian==1){jian=0;change=ds[2];change=change-1;ds[2]=change;}
					yi=ds[2]/10;er=ds[2]%10;san=10;si=ds[1]/10;wu=ds[1]%10;liu=10;qi=ds[0]/10;ba=ds[0]%10;
				}
					else if(shijian[0]%2==1)
				{
					  if(jia==1){jia=0;change=ds[2];change=change+1;ds[2]=change;}
						if(jian==1){jian=0;change=ds[2];change=change-1;ds[2]=change;}
						yi=11;er=11;san=10;si=ds[1]/10;wu=ds[1]%10;liu=10;qi=ds[0]/10;ba=ds[0]%10;
				}
      
			}
		
			else if(k==3)
			{
				if(shijian[0]%2==0)
				{
					if(jia==1){jia=0;change=ds[1];change=change+1;ds[1]=change;}
					if(jian==1){jian=0;change=ds[1];change=change-1;ds[1]=change;}
					yi=ds[2]/10;er=ds[2]%10;san=10;si=ds[1]/10;wu=ds[1]%10;liu=10;qi=ds[0]/10;ba=ds[0]%10;
				}
				else if(shijian[0]%2==1)
				{
					if(jia==1){jia=0;change=ds[1];change=change+1;ds[1]=change;}
					if(jian==1){jian=0;change=ds[1];change=change-1;ds[1]=change;}
					yi=ds[2]/10;er=ds[2]%10;san=10;si=11;wu=11;;liu=10;qi=ds[0]/10;ba=ds[0]%10;
				}
				

				}				
		
			else if(k==4)	
			 {
					if(shijian[0]%2==0)
					{
							if(jia==1){jia=0;ds[0]++;}
						if(jian==1){jian=0;ds[0]--;}
							yi=ds[2]/10;er=ds[2]%10;san=10;si=ds[1]/10;wu=ds[1]%10;liu=10;qi=ds[0]/10;ba=ds[0]%10;
					}
					else if(shijian[0]%2==1)
					{
							if(jia==1){jia=0;ds[0]++;}
						if(jian==1){jian=0;ds[0]--;}
								yi=ds[2]/10;er=ds[2]%10;san=10;si=ds[1]/10;wu=ds[1]%10;liu=10;qi=11;ba=11;
					}ds_init();

					
			 }

	 }
		 if(nao==1)
		{	 
    if(q==1)//显示
		{
		
			
	    yi=naozhong[2]/10;er=naozhong[2]%10;san=10;
			si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;
			qi=naozhong[0]/10;ba=naozhong[0]%10;


		}
		else if(q==2)//设置时
		{
			  if(jia==1){jia=0;naozhong[2]++;}
				if(jian==1){jian=0;naozhong[2]--;}
				if(shijian[0]%2==0){yi=naozhong[2]/10;er=naozhong[2]%10;san=10;si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;qi=naozhong[0]/10;ba=naozhong[0]%10;}
				else if(shijian[0]%2==1){yi=11;er=11;san=10;si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;qi=naozhong[0]/10;ba=naozhong[0]%10;}

			}

	
   else if(q==3)
		{
		    if(jia==1){jia=0;naozhong[1]++;}
				if(jian==1){jian=0;naozhong[1]--;}
				if(shijian[0]%2==0){yi=naozhong[2]/10;er=naozhong[2]%10;san=10;si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;qi=naozhong[0]/10;ba=naozhong[0]%10;}
				else if(shijian[0]%2==1){yi=naozhong[2]/10;er=naozhong[2]%10;san=10;si=11;wu=11;liu=10;qi=naozhong[0]/10;ba=naozhong[0]%10;}

		}	
	else if(q==4)
		{
		    if(jia==1){jia=0;naozhong[0]++;}
				if(jian==1){jian=0;naozhong[0]--;}
				if(shijian[0]%2==0){yi=naozhong[2]/10;er=naozhong[2]%10;san=10;si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;qi=naozhong[0]/10;ba=naozhong[0]%10;}
				else if(shijian[0]%2==1){yi=naozhong[2]/10;er=naozhong[2]%10;san=10;si=naozhong[1]/10;wu=naozhong[1]%10;liu=10;qi=11;ba=11;}

		}
	}
		if((naozhong[2]==shijian[2])&&(naozhong[1]==shijian[1])&&(naozhong[0]==shijian[0]))
		{
			EA=1;ET0=1;
			P2=0X80;P0=0X00;
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
			shizhong=1;nao=0;
			switch(k)
			{
				case 1:k=2;break;
				case 2:k=3;break;
				case 3:k=4;break;
				case 4:k=1;break;
				
			}
		}while(!P30);
	}
	if(P31==0)//s6
	{
		delayms(5);
		if(P31==0)
		{
			nao=1;shizhong=0;
			switch(q)
			{
				case 1:q=2;break;
				case 2:q=3;break;
				case 3:q=4;break;
				case 4:q=1;break;
				
			}
		}while(!P31);
	}
	if(P32==0)//s5
	{
		delayms(5);
		if(P32==0)
		{
			jia=1;
		}while(!P32);
	}
	if(P33==0)//s4
	{
		delayms(5);
		if(P33==0)
		{
			jian=1;
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
	
}
void timer0() interrupt 1
{
	t++;
	if(t==200)
	{
		t=0;
		P2=0X80;P0=0XFF;
	
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

/*温度模块*/
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
	uchar temp,low,high;
	
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

/*时钟模块*/
uchar read(uchar add)
{
	uchar i,temp;
	uchar dat1,dat2;
	CE=0;
	SCLK=0;
	CE=1;
	writebyte(add);
	for(i=0;i<8;i++)
	{
		SCLK=0;
		temp>>=1;
		if(IO)
		{
			temp|=0x80;
		}
		SCLK=1;
	}
	
	IO=0;
	
	dat1=temp/16;//0XFF 1111 1111
	dat2=temp%16;
	
	temp=dat1*10+dat2;
	
	return temp;
}


void writebyte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCLK=0;
		IO=dat&0x01;
		SCLK=1;
		dat>>=1;
	}
}

void write(uchar add,uchar dat)
{
	uchar num;
	
	CE=0;
	SCLK=0;
	CE=1;
	writebyte(add);
	num=(dat/10<<4)|(dat%10);// 55
	writebyte(num);
	CE=0;
}
void ds_init()
{
	uchar i,dat=0x80;
	write(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		
		write(dat,ds[i]);
		dat=dat+2;
	}
	write(0x8e,0x80);
}
void ds_get()
{
	uchar i,add=0x81;
	write(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		
		shijian[i]=read(add);
		add=add+2;
	}
	write(0x8e,0x80);
}





