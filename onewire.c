/*
  ����˵��: ��������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨
  ��    ��: 2011-8-9
*/


#include "onewire.h"
//��������ʱ����
void Delay_OneWire(unsigned int t)
{
  while(t--);
}

//DS18B20оƬ��ʼ��
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

//ͨ����������DS18B20дһ���ֽ�
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

//��DS18B20��ȡһ���ֽ�
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
	Delay_OneWire(5);
	
	Init_DS18B20();
	Write_DS18B20(0XCC);
	Write_DS18B20(0XBE);

	low=Read_DS18B20();
	high=Read_DS18B20();
	
	temp=high<<4;
	temp|=(low>>4);
	
	return temp;
}