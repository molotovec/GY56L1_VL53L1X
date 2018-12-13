#include <reg52.h>
#include "usart.h"
#include "iic.h"  
#include "string.h"
#include "stdio.h"
/*
硬件接法：
GY-53L1---c51
1、GY-53_RX---c51_TX   复位单片机，单片机发送自动输出指令（或通过上位机与模块连接设置模块自动输出）
2、c51_TX---FT232,将数据串口打印输出
3、GY_53L1_TX---c51_RX，接收模块数据
软件说明:
该程序采用串口方式获取模块数据，波特率9600

注：中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 function,u8 command)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=function;//功能字节
    bytes[2]=command; //值
	USART_Send(bytes,4);//发送帧头、功能字节、校验和
}
 void delay(uint16_t x)
 {
   while(x--);
 }
int main(void)
{

    u8 sum=0,i=0,temp=0;
	u8 RangeStatus=0,Time=0,Mode=0;
	uint16_t distance=0;
	Usart_Int(9600);
	delay(10000);
	send_com(0x65,0x01);  //自动输出
 	while(1)
	{	   
		if(Receive_ok)//串口接收完毕
		{	
			for(sum=0,i=0;i<(re_Buf_Data[3]+4);i++)//rgb_data[3]=3
			sum+=re_Buf_Data[i];
			if(sum==re_Buf_Data[i])//校验和判断
			{
				distance=re_Buf_Data[4]<<8|re_Buf_Data[5];
				Mode=re_Buf_Data[6]&0x03;	 
				Time=(re_Buf_Data[6]>>2)&0x03;	   
				RangeStatus=(re_Buf_Data[6]>>4)&0x0f;
			
			
				//send_3out(&re_Buf_Data[4],3,0x15);//上传给上位机
				printf("distance: %d,RangeStatus: %bd,Mode: %bd,Time: %bd\r\n",distance,RangeStatus,Mode,Time);
			}
			Receive_ok=0;//处理数据完毕标志
		}	
	}
}
