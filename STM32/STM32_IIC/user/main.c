#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY-56L1---STM32
SCL---PB6
SDA---PB7
STM32 USART1---FT232
TX---RX
RX---TX
���˵��:

ע��
	�жϺ���λ��stm32f10x_it.c
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
	NVIC_X.NVIC_IRQChannel = USART1_IRQn;//�ж�����
  NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
  NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
  NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
  NVIC_Init(&NVIC_X);
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(!((USART1->SR)&(1<<7)));//ѭ������,ֱ���������   
	USART1->DR = (uint8_t) ch;      
	return ch;
}
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
	uint16_t distance=0;
  u8 i=0;
	u8 ADDR=0Xe0;
	uint16_t delay_time=0;
	u8 measure_time=measure_time_100ms;//��� ʱ��Ϊ100ms
	delay_init(72);
	LED_Int(GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB);
	NVIC_Configuration();
	Usart_Int(9600);
	I2C_GPIO_Config();
	 
	 //delay_ms(100);
	// changeAddress(ADDR,0x40);//����IIC��ַ��ģ�齫���������
	changemode(ADDR,DISTANCEMODE_MEDIUM,measure_time);//�޸Ĳ��ģʽ��ʱ�䣬ģ�鲻���������
	switch(measure_time)//ѡ���ȡ���
	{
		case measure_time_50ms:delay_time=100;break;
		case measure_time_100ms:delay_time=150;break;
		case measure_time_200ms:delay_time=250;break;
		case measure_time_300ms:delay_time=350;break;
	}
	 delay_ms(100);//�ȴ�ģ���ʼ�����
	while(1)
	{

		requestRange((ADDR+1),&distance);//��ȡ����
		takeRangeReading(ADDR);//���Ͳ��ָ��

		//send_out(&diatance,1,0x45);
		printf("diatance:%d\r\n",distance);//����1��ӡ���
		delay_ms(delay_time);//��ȡ���
		
	}
}
