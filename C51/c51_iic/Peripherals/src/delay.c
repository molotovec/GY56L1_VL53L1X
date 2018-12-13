 #include "delay.h"
 u8 Time_count=0;
 uint16_t Delay_count=0;
 uint16_t Time_ms=0;	  
 #define system_clk  11.0592   //系统晶振
 //x：需要中断的时间 us ；x< 277  us
 void Delay_Init(uint16_t x)
 {
	 Time_count=256-(x*system_clk/12);
	 Time_ms=1000/x;
	 TMOD |= 0x02;//定时器0，8位自动重装计数值
     TH0  =Time_count;
	 TL0  =Time_count;
     //TCON |= 0x10;//定时器允许计时
	 TR0=0;
	 IP&= 0xfd;//定时器0低优先级
	 //IE   |=0x82;//允许定时器0中断，MCU总中断
	 ET0=1;
	 EA=1;
	      
 }
 void delay_ms(uint16_t x)
 {
     
     Delay_count=x*Time_ms;
	 TR0=1;//定时器0开始计数
   	 while(Delay_count);
	 TR0=0;  // 停止计数
	 TL0 =Time_count; //重新赋初值
	 x=TF0;	//TF0查询清零
 }