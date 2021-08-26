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
void Freq_display_init();
void Timer0Init(void)	;
uchar t,Freq_val;
void main()
{
	allinit();Timer0Init();	
	while(1)
	{
		
		Freq_display_init();
		yi=Freq_val/10000;er=Freq_val%10000/1000;san=Freq_val%1000/100;si=Freq_val%100/10;wu=Freq_val%10;
		
		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
	}
}

void Timer0Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
  EA=1;ET0=1;
}
void timer0() interrupt 1
{
	t++;
}
void Freq_display_init()
{
   if(t==100)
   {
	 t=0;
	 TR0 = 0;								//�رն�ʱ��0����
	 Freq_val = TH0 * 256 + TL0;	//ͳ��500ms��������
	 Freq_val *= 2;								//����2����1s��������������Ƶ��
	 TH0 = TL0 = 0;					//�����ʱ��0��������
	 TR0 = 1; 								//��ʱ��0���¿�ʼ����
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