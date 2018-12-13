#ifndef __DELAY_H__
#define __DELAY_H__
#include <reg52.h>
#define u8 unsigned char
#define uint16_t  unsigned int
void Delay_Init(uint16_t x);
void delay_ms(uint16_t x);
 extern uint16_t Delay_count;
#endif