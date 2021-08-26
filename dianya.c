#include<STC15F2K60S2.H>
#include<intrins.h>

#define uchar unsigned char 
#define uint unsigned int	
#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1	

sbit SCL=P2^0;
sbit SDA=P2^1;

uchar code tab[]={0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90,0XBF,0XFF};
uchar yi,er,san,si,wu ,liu,qi,ba,T;
uchar dianya;

void delayms(int ms);
void Delay50ms();
void allinit();
void display1(uchar yi,uchar er);
void display2(uchar san,uchar si);
void display3(uchar wu,uchar liu);
void display4(uchar qi,uchar ba);



void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt);
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
void AD_write(uchar add,uchar dat);
uchar AD_read(uchar add);

void select(unsigned char n);
void close();
void Timer0Init(void)	;
void rbinit();

sbit relay=P0^4;
sbit buzz=P0^6;
sbit l1=P0^0;
sbit l2=P0^1;
sbit l3=P0^2;

void select(unsigned char n)
{
	P2=P2&0x1f;
	P2=P2|(n<<5);
}
void close()
{
	P2=P2&0x1f;
}


void rbinit()
{
	relay=0;
	buzz=0;
	select(5);
	close();
}
void Timer0Init(void)		//5����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	EA=1;ET0=0;
}


void main()
{
	allinit();
	Timer0Init();
	yi=11;er=11;san=11;si=11;wu=11;liu=11;qi=11;ba=11;
	while(1)
	{

		dianya=AD_read(0X03);
    
		
		liu=dianya/100;qi=dianya%100/10;ba=dianya%10;
		if(dianya<50)
		{	
			ET0=0;
			allinit();
      P2=0X80;P0=0XFE;
    }			
			
		if((dianya<100)&&(dianya>50))
			{
			ET0=0;
			allinit();
				P2=0X80;P0=0XFD;

			}
		if(dianya>100)
			{
         ET0=1;
				
      
			}

		display1(yi,er);
		display2(san,si);
		display3(wu,liu);
		display4(qi,ba);
	
	}


}



void isr0() interrupt 1
{
	T++;	
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;	
	if(T==50)
	{
		T=0;

	  l3=~l3;		P2=0X80;P00=1;P01=1;P03=1;P04=1;P05=1;P06=1;P07=1;
	}
	
}
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	somenop;
	SDA = 1;
}
//�ȴ�Ӧ��
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

//ͨ��I2C���߷�������
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

//��I2C�����Ͻ�������
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







uchar AD_read(uchar add)
{
	uchar temp;
	
	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0X91);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_Stop();
	
	
	return temp;
	
}

void AD_write(uchar add,uchar dat)
{

	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
  IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
	

}


void DAC(uchar dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(0x40);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();

}

void delayms(int ms)
{
	int i,j;
	for(i=ms;i>0;i--)
		for(j=845;j>0;j--);
}

void allinit()
{
	P2=0XA0;
	P0=0X00;//�رշ��������̵���
	
	P2=0X80;
	P0=0XFF;//�ر�LED��
	
	P2=0XC0;
	P0=0XFF;//ѡ�����������
	P2=0XFF;
	P0=0XFF;//�ر����������
}

void display1(uchar yi,uchar er)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X01;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[yi];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X02;//ѡ��ڶ��������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[er];
		delayms(1);
}	

void display2(uchar san,uchar si)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X04;//ѡ������������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[san];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X08;//ѡ����ĸ������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[si];
		delayms(1);
}

void display3(uchar wu,uchar liu)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X10;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[wu];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X20;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[liu];
		delayms(1);
}

void display4(uchar qi,uchar ba)
{
		P2=0XC0;//��λѡ573   U8
		P0=0X40;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[qi];
		delayms(1);
		
		P2=0XC0;//��λѡ573   U8
		P0=0X80;//ѡ���һ�������
		P2=0XFF;//�򿪶�ѡ573   U7
		P0=tab[ba];
		delayms(1);
}



