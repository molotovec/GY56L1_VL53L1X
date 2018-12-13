/*
GY56-----------MINI
VCC ---------- 5V
GND  --------- GND
Pin 5 -------- RC
Pin 4 -------- TD
Pin PS-------GND
*/
//Display libs
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//
#include "Wire.h"
//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8-bit 0xE0
#define SensorAddress byte(0x70)
//The sensors ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)
#define ChangModeCommand1  byte(0x5A)
#define ChangModeCommand2  byte(0x55)
#define measure_time_50ms 0
#define measure_time_100ms 1
#define measure_time_200ms 2
#define measure_time_300ms 3
#define DISTANCEMODE_SHORT  1
#define DISTANCEMODE_MEDIUM 2
#define DISTANCEMODE_LONG   3
uint16_t delay_time=0;

Adafruit_SSD1306 display = Adafruit_SSD1306(); //display declartion

void setup() {
 uint16_t measure_time=measure_time_50ms;
 Serial.begin(115200); //Open serial connection at 9600 baud
 Wire.begin(); 
 change_mode(SensorAddress,DISTANCEMODE_LONG,measure_time);
 switch(measure_time)
 {
   case 0:delay_time=100;break;
   case 1:delay_time=150;break;
   case 2:delay_time=250;break;
   case 3:delay_time=350;break;
 }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.display();
  // text display big!
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
 delay(1000);
}
void loop(){
 takeRangeReading(); //Tell the sensor to perform a ranging cycle
 delay(delay_time); //Wait for sensor to finish
  word range = requestRange(); //Get the range from the sensor
  Serial.print("Range: "); Serial.print(range); Serial.println(" cm"); //Print to the user
  //To Display
  displayPlay(range);
}
//Commands the sensor to take a range reading
void takeRangeReading(){
  Wire.beginTransmission(SensorAddress); //Start addressing
  Wire.write(RangeCommand); //send range command
  Wire.endTransmission(); //Stop and do something else now
}
//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.
word requestRange(){
  Wire.requestFrom(SensorAddress, byte(2));
  if(Wire.available() >= 2){ //Sensor responded with the two bytes
  byte HighByte = Wire.read(); //Read the high byte back
  byte LowByte = Wire.read(); //Read the low byte back
  word range = word(HighByte, LowByte); //Make a 16-bit word out of the two bytes for the range
  return range;
}
else {
  return word(0); //Else nothing was received, return 0
}
}
/* Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
If true, if is the 7 bit version, if false, it is the 8 bit version
*/
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh){
Wire.beginTransmission(oldAddress); //Begin addressing
Wire.write(ChangeAddressCommand1); //Send first change address command
Wire.write(ChangeAddressCommand2); //Send second change address command
byte temp;
if(SevenBitHuh){ temp = newAddress << 1; } //The new address must be written to the sensor
else { temp = newAddress; } //in the 8bit form, so this handles automatic shifting
Wire.write(temp); //Send the new address to change to
Wire.endTransmission();
}
void change_mode(byte Address, byte mode, byte  time){
Wire.beginTransmission(Address); //Begin addressing
Wire.write(ChangModeCommand1); //Send first  command
Wire.write(ChangModeCommand2); //Send second  command
byte temp;
temp=(mode<<4)|time;
Wire.write(temp); //Send the new address to change to
Wire.endTransmission();
}
void displayPlay(int sensorRange){
  display.clearDisplay();
      display.setCursor(0,0);
      display.print("Range: ");
      display.print(sensorRange);
      display.print(" cm");
      display.display();
      Serial.println();
      delay(50);
}
