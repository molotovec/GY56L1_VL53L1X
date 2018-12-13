#include <reg52.h>
#include "usart.h"
#include "iic.h"  
#include "string.h"
#include "stdio.h"
/*
Ӳ���ӷ���
GY-53L1---c51
1��GY-53_RX---c51_TX   ��λ��Ƭ������Ƭ�������Զ����ָ���ͨ����λ����ģ����������ģ���Զ������
2��c51_TX---FT232,�����ݴ��ڴ�ӡ���
3��GY_53L1_TX---c51_RX������ģ������
���˵��:
�ó�����ô��ڷ�ʽ��ȡģ�����ݣ�������9600

ע���жϺ���λ��stc_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
void send_com(u8 function,u8 command)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=function;//�����ֽ�
    bytes[2]=command; //ֵ
	USART_Send(bytes,4);//����֡ͷ�������ֽڡ�У���
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
	send_com(0x65,0x01);  //�Զ����
 	while(1)
	{	   
		if(Receive_ok)//���ڽ������
		{	
			for(sum=0,i=0;i<(re_Buf_Data[3]+4);i++)//rgb_data[3]=3
			sum+=re_Buf_Data[i];
			if(sum==re_Buf_Data[i])//У����ж�
			{
				distance=re_Buf_Data[4]<<8|re_Buf_Data[5];
				Mode=re_Buf_Data[6]&0x03;	 
				Time=(re_Buf_Data[6]>>2)&0x03;	   
				RangeStatus=(re_Buf_Data[6]>>4)&0x0f;
			
			
				//send_3out(&re_Buf_Data[4],3,0x15);//�ϴ�����λ��
				printf("distance: %d,RangeStatus: %bd,Mode: %bd,Time: %bd\r\n",distance,RangeStatus,Mode,Time);
			}
			Receive_ok=0;//����������ϱ�־
		}	
	}
}
