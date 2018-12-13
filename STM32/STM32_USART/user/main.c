#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
 #include "LED.h"
 /*
 Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY-56l1---STM32
1��GY-56l1_RX---STM32_TX2(PIN_A2),STM32��λ������ ����������ú��Զ����ָ�� ��ģ��
2��GY-56l1_TX---STM32_RX2(PIN_A3)
3��STM32_TX1(PIN_A9)---FT232_RX,STM32�����ڴ�ӡ��������ͨ���������ֲ鿴

���˵��:
�ó�����ô��ڷ�ʽ��ȡģ�����ݣ�������9600
ע���жϺ���λ��stm32f10x_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_X;
  
  /* 4����ռ���ȼ���4����Ӧ���ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /*��ռ���ȼ��ɴ���жϼ���͵��ж�*/
	/*��Ӧ���ȼ����ȼ�ִ��*/
	NVIC_X.NVIC_IRQChannel = USART2_IRQn;//�ж�����
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
  NVIC_Init(&NVIC_X);
}

void send_com(u8 function,u8 command)
{
	u8 bytes[4]={0};
	bytes[0]=0xa5;
	bytes[1]=function;//�����ֽ�
  bytes[2]=command; //ֵ
	USART_Send(bytes,4);//����֡ͷ�������ֽڡ�У���
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(!((USART1->SR)&(1<<7)));//ѭ������,ֱ���������   
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
	delay_ms(500);//�ȴ�ģ���ʼ�����
	send_com(0x65,0x01); //�Զ����
	while(1)
	{
		if(Receive_ok)//���ڽ������
		{
			for(sum=0,i=0;i<(re_Buf_Data[3]+4);i++)//rgb_data[3]=3
			sum+=re_Buf_Data[i];
			if(sum==re_Buf_Data[i])//У����ж�
			{
				distance=re_Buf_Data[4]<<8|re_Buf_Data[5];
				RangeStatus=(re_Buf_Data[6]>>4)&0x0f;
				Time=(re_Buf_Data[6]>>2)&0x03;
				Mode=re_Buf_Data[6]&0x03;
				//send_3out(&re_Buf_Data[4],3,0x15);//�ϴ�����λ��
					printf("distance: %d,RangeStatus: %d,Mode: %d,Time: %d\r\n",distance,RangeStatus,Mode,Time);
			}
			Receive_ok=0;//����������ϱ�־
		}
	}
}
