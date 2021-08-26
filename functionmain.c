#include<STC15F2K60S2.h>
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

void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);
void delayms(uint ms)	;
void delay1ms();
void allinit();
void keyscan();
void keyscan16();							

void main()
{
  allinit();
	yi=1;er=2;san=3;si=4;wu=5;liu=6;qi=7;ba=8;
	while(1)
	{

		P2=0X80;		keyscan16();
	 display1(yi,er);
   display2(san,si);
   display3(wu,liu);
   display4(qi, ba);
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
		  yi=8;
		}while(!P30);			
	}
 else if(P31==0)
	{
		delayms(5);
    if(P31==0)
		{
		  er=9;
		}while(!P31);			
	}
 else if(P32==0)
	{
		delayms(5);
    if(P32==0)
		{
		  san=9;
		}while(!P32);			
	}
 else if(P33==0)
	{
		delayms(5);
    if(P33==0)
		{
		  si=9;
		}while(!P33);			
	}


}






void keyscan16()
{
	uchar temp;
	P42=1;P44=0;P3=0X7F;
	temp=P3;temp=temp&0X0F;
	if(temp!=0X0F)
	{
		delayms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0X7E:yi=10;er=2;break;
				case 0X7D:P0=~0X22;break;
				case 0X7B:P0=~0X33;break;
				case 0X77:P0=~0X44;break;
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
		delayms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0XBE:P0=~0X01;break;
				case 0XBD:P0=~0X02;break;
				case 0XBB:P0=~0X04;break;
				case 0XB7:P0=~0X08;break;
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
		delayms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0XDE:P0=~0X01;break;
				case 0XDD:P0=~0X02;break;
				case 0XDB:P0=~0X04;break;
				case 0XD7:P0=~0X08;break;
			}while(temp!=0x0F)
			{
			  temp=P3;temp=temp&0X0F;
			}
				
		
		}

	}
	
	P42=1;P44=1;P3=0XEF;
	temp=P3;temp=temp&0X0F;
	if(temp!=0X0F)
	{
		delayms(5);
		temp=P3;temp=temp&0X0F;
		if(temp!=0X0F)
		{
			temp=P3;
			switch(temp)
			{
				case 0XEE:P0=~0X01;break;
				case 0XED:P0=~0X02;break;
				case 0XEB:P0=~0X04;break;
				case 0XE7:P0=~0X08;break;
			}while(temp!=0x0f)
			{
			  temp=P3;temp=temp&0X0F;
			}
				
		
		}

	}

}


