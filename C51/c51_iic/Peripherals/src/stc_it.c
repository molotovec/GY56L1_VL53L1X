#include "stc_it.h"
#include "usart.h"
#include "delay.h"


void TIME0_IRQHandler(void)interrupt 1  
{
  if(Delay_count)
   Delay_count--;
  // LED_0 =!LED_0; 
}
//�ⲿ�ж�1
void EXTI1_IRQHandler(void)interrupt 2
{
}
//�����ж�
void USART_IRQHandler(void)interrupt 4
{
	static uint8_t i=0,rebuf[3]={0};
	uint8_t sum=0;
    if(TI)//������ɱ�־
	{
	  TI=0;//�巢����ɱ�־
	  send_ok=0;//�����־��0 
	}
	if(RI)//������ɱ�־
	{
		rebuf[i++]=SBUF;
		RI=0;//���жϽ��ձ�־
		if(!(rebuf[0]==0xa5))//�ж�֡ͷ
		i=0;
		if(i==3)
		{
			sum=rebuf[0]+rebuf[1];
			if(sum==rebuf[2])
			{
			/*	if(rebuf[1]==0xbb)//У׼ָ��
				{
					Calibration=1;
				 
				}
				if(rebuf[1]==0x51)//������
				{
					Calibration=0;
					
				}	*/
			}
			i=0;
		}
	
	}

}
