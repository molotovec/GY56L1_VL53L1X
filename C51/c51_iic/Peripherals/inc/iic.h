#ifndef __IIC_H__
#define __IIC_H__
#include <reg52.h>
#define u8 unsigned char
#define uint16_t  unsigned int
sbit SCL=P3^6;
sbit SDA=P3^7;
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length);
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 datas);
u8 requestRange(u8 Slave_Address,uint16_t *distance);
u8 takeRangeReading(u8 Slave_Address);
u8 changemode(u8 Address, u8 mode,u8 time);
u8 changeAddress(u8 oldAddress, u8 newAddress);
#endif
