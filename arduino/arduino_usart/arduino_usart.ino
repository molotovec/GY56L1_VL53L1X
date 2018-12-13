/////////////////////
/*
GY56L1----MINI
VCC----VCC
GND----GND
1:GY56L1_TX---10
2:GY56L1_RX---11
*/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

//////////////////
unsigned char re_Buf[11],counter=0;
unsigned char sign=0;
byte data_Buf[3]={0};
void setup() {
   Serial.begin(9600);  
   mySerial.begin(9600);
   mySerial.listen();  
  delay(100);    
  mySerial.write(0XA5); 
  mySerial.write(0X65);    //初始化,连续输出模式
  mySerial.write(0X01);    //初始化,连续输出模式
  mySerial.write(0X0b);    //初始化,连续输出模式
}

void loop() {
  unsigned char i=0,sum=0;
   unsigned char RangeStatus=0,Time=0,Mode=0;
  uint16_t distance=0;
   while (mySerial.available()) {   
    re_Buf[counter]=(unsigned char)mySerial.read();
    if(counter==0&&re_Buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==8)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
  if(sign)
  {   
     sign=0;
     for(i=0;i<7;i++)
      sum+=re_Buf[i]; 
     if(sum==re_Buf[i] )        //检查帧头，帧尾
     {  	       
          data_Buf[0]=re_Buf[4];
          data_Buf[1]=re_Buf[5];
          data_Buf[2]=re_Buf[6];
          distance=data_Buf[0]<<8|data_Buf[1];
          RangeStatus=(data_Buf[2]>>4)&0x0f;
	  Time=(data_Buf[2]>>2)&0x03;
	  Mode=data_Buf[2]&0x03;
           Serial.print("distance: ");
           Serial.print( distance);
           Serial.print(" mm,RangeStatus: ");
           Serial.print( RangeStatus);  
           Serial.print(" ,Time: ");
           Serial.print( Time);   
           Serial.print(",Mode: ");
           Serial.println( Mode);      
   }
  } 

}
/*void serialEvent() {
  while (Serial.available()) {   
    re_Buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&re_Buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==8)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
}*/
