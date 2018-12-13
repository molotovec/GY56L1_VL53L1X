#include <reg52.h>
#include "usart.h"
#include "iic.h"
#include "delay.h"
#include "string.h"  
#include "stdio.h"

/*
Ӳ���ӷ���
GY-56L1----C51
SCL---P3^6
SDA---P3^7
C51---FT232
TX ---RX
RX ---TX
���˵����

ע��
	�жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
/*���ʱ��*/
#define measure_time_50ms 0
#define measure_time_100ms 1
#define measure_time_200ms 2
#define measure_time_300ms 3
/*���ģʽ*/
#define DISTANCEMODE_SHORT  1
#define DISTANCEMODE_MEDIUM 2
#define DISTANCEMODE_LONG   3

int main(void)
{	
   uint16_t delay_time=0;
   u8 measure_time=measure_time_100ms;//��� ʱ��Ϊ100ms
	Usart_Int(9600);
	Delay_Init(200);  //  200us�ж�һ��
   // changeAddress(0xe0,0x40);//����IIC��ַ��ģ�齫���������
	changemode(0xe0,DISTANCEMODE_MEDIUM,measure_time);//�޸Ĳ��ģʽ��ʱ�䣬ģ�鲻���������
	switch(measure_time)//ѡ���ȡ���
	{
		case measure_time_50ms:delay_time=100;break;
		case measure_time_100ms:delay_time=150;break;
		case measure_time_200ms:delay_time=250;break;
		case measure_time_300ms:delay_time=350;break;
	}
 	while(1)
	{
	   	uint16_t diatance=0;
	    requestRange((0xe1),&diatance);		 //��ȡ��������
		takeRangeReading(0xe0);				 //���Ͳ�������
		
		//send_out(&diatance,1,0x45);		 //������λ��
		printf("diatance:%d\r\n",diatance);//����1��ӡ���
		delay_ms(delay_time);		      //��ȡ���
	}
}
