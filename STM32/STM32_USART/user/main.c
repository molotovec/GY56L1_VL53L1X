#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
 #include "LED.h"
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-56l1---STM32
1、GY-56l1_RX---STM32_TX2(PIN_A2),STM32复位将发送 输出数据配置和自动输出指令 给模块
2、GY-56l1_TX---STM32_RX2(PIN_A3)
3、STM32_TX1(PIN_A9)---FT232_RX,STM32将串口打印出来，可通过串口助手查看

软件说明:
该程序采用串口方式获取模块数据，波特率9600
注：中断函数位于stm32f10x_it.c
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
	NVIC_X.NVIC_IRQChannel = USART2_IRQn;//中断向量
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
  NVIC_Init(&NVIC_X);
}

void send_com(u8 function,u8 command)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=function;//功能字节
  bytes[2]=command; //值
	USART_Send(bytes,4);//发送帧头、功能字节、校验和
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(!((USART1->SR)&(1<<7)));//循环发送,直到发送完毕   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
int main(void)
{
  u8 sum=0,i=0;
	u8 RangeStatus=0,Time=0,Mode=0;
	int16_t data=0;
	uint16_t distance=0;
	delay_init(72);
	NVIC_Configuration();
	Usart_Int(9600);
	Usart_Int2(9600);
	delay_ms(500);//等待模块初始化完成
	send_com(0x65,0x01); //自动输出
	while(1)
	{
		if(Receive_ok)//串口接收完毕
		{
			for(sum=0,i=0;i<(re_Buf_Data[3]+4);i++)//rgb_data[3]=3
			sum+=re_Buf_Data[i];
			if(sum==re_Buf_Data[i])//校验和判断
			{
				distance=re_Buf_Data[4]<<8|re_Buf_Data[5];
				RangeStatus=(re_Buf_Data[6]>>4)&0x0f;
				Time=(re_Buf_Data[6]>>2)&0x03;
				Mode=re_Buf_Data[6]&0x03;
				//send_3out(&re_Buf_Data[4],3,0x15);//上传给上位机
					printf("distance: %d,RangeStatus: %d,Mode: %d,Time: %d\r\n",distance,RangeStatus,Mode,Time);
			}
			Receive_ok=0;//处理数据完毕标志
		}
	}
}
