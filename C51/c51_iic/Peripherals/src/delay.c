 #include "delay.h"
 u8 Time_count=0;
 uint16_t Delay_count=0;
 uint16_t Time_ms=0;	  
 #define system_clk  11.0592   //ϵͳ����
 //x����Ҫ�жϵ�ʱ�� us ��x< 277  us
 void Delay_Init(uint16_t x)
 {
	 Time_count=256-(x*system_clk/12);
	 Time_ms=1000/x;
	 TMOD |= 0x02;//��ʱ��0��8λ�Զ���װ����ֵ
     TH0  =Time_count;
	 TL0  =Time_count;
     //TCON |= 0x10;//��ʱ�������ʱ
	 TR0=0;
	 IP&= 0xfd;//��ʱ��0�����ȼ�
	 //IE   |=0x82;//����ʱ��0�жϣ�MCU���ж�
	 ET0=1;
	 EA=1;
	      
 }
 void delay_ms(uint16_t x)
 {
     
     Delay_count=x*Time_ms;
	 TR0=1;//��ʱ��0��ʼ����
   	 while(Delay_count);
	 TR0=0;  // ֹͣ����
	 TL0 =Time_count; //���¸���ֵ
	 x=TF0;	//TF0��ѯ����
 }