#ifndef _IIC_H
#define _IIC_H

//��������
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
void  EEPROM_write(unsigned char add,unsigned char dat);
unsigned char  EEPROM_read(unsigned char add);
unsigned char  AD_read(unsigned char add);
#endif