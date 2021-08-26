#ifndef __DS1302_H
#define __DS1302_H

void Write_Ds1302_Byte(unsigned  char temp);
void Write_Ds1302( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302 ( unsigned char address );
void get_ds1302();
void ds1302init();
void  EEPROM_write(unsigned char add,unsigned char dat);
unsigned char  EEPROM_read(unsigned char add);
unsigned char  AD_read(unsigned char add);
#endif
