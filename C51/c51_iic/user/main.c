#include <reg52.h>
#include "usart.h"
#include "iic.h"
#include "delay.h"
#include "string.h"  
#include "stdio.h"

/*
硬件接法：
GY-56L1----C51
SCL---P3^6
SDA---P3^7
C51---FT232
TX ---RX
RX ---TX
软件说明：

注：
	中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
/*测距时间*/
#define measure_time_50ms 0
#define measure_time_100ms 1
#define measure_time_200ms 2
#define measure_time_300ms 3
/*测距模式*/
#define DISTANCEMODE_SHORT  1
#define DISTANCEMODE_MEDIUM 2
#define DISTANCEMODE_LONG   3

int main(void)
{	
   uint16_t delay_time=0;
   u8 measure_time=measure_time_100ms;//测距 时间为100ms
	Usart_Int(9600);
	Delay_Init(200);  //  200us中断一次
   // changeAddress(0xe0,0x40);//更改IIC地址，模块将保存该设置
	changemode(0xe0,DISTANCEMODE_MEDIUM,measure_time);//修改测距模式和时间，模块不保存该配置
	switch(measure_time)//选择读取间隔
	{
		case measure_time_50ms:delay_time=100;break;
		case measure_time_100ms:delay_time=150;break;
		case measure_time_200ms:delay_time=250;break;
		case measure_time_300ms:delay_time=350;break;
	}
 	while(1)
	{
	   	uint16_t diatance=0;
	    requestRange((0xe1),&diatance);		 //读取距离数据
		takeRangeReading(0xe0);				 //发送测量命令
		
		//send_out(&diatance,1,0x45);		 //发向上位机
		printf("diatance:%d\r\n",diatance);//串口1打印输出
		delay_ms(delay_time);		      //读取间隔
	}
}
