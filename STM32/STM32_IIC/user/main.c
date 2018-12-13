#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-56L1---STM32
SCL---PB6
SDA---PB7
STM32 USART1---FT232
TX---RX
RX---TX
软件说明:

注：
	中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4个抢占优先级，4个响应优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*抢占优先级可打断中断级别低的中断*/
	/*响应优先级按等级执行*/
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//中断向量
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
  NVIC_Init(&NVIC_X);
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(!((USART1->SR)&(1<<7)));//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
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
	uint16_t distance=0;
  u8 i=0;
	u8 ADDR=0Xe0;
	uint16_t delay_time=0;
	u8 measure_time=measure_time_100ms;//测距 时间为100ms
	delay_init(72);
	LED_Int(GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB);
	NVIC_Configuration();
	Usart_Int(9600);
	I2C_GPIO_Config();
	 
	 //delay_ms(100);
	// changeAddress(ADDR,0x40);//更改IIC地址，模块将保存该设置
	changemode(ADDR,DISTANCEMODE_MEDIUM,measure_time);//修改测距模式和时间，模块不保存该配置
	switch(measure_time)//选择读取间隔
	{
		case measure_time_50ms:delay_time=100;break;
		case measure_time_100ms:delay_time=150;break;
		case measure_time_200ms:delay_time=250;break;
		case measure_time_300ms:delay_time=350;break;
	}
	 delay_ms(100);//等待模块初始化完成
	while(1)
	{

		requestRange((ADDR+1),&distance);//读取数据
		takeRangeReading(ADDR);//发送测距指令

		//send_out(&diatance,1,0x45);
		printf("diatance:%d\r\n",distance);//串口1打印输出
		delay_ms(delay_time);//读取间隔
		
	}
}
