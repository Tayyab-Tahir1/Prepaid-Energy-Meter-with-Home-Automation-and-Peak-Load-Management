#include <OLED_I2C.h>
#include "EmonLib.h"

EnergyMonitor emon1;
OLED  myOLED(SDA, SCL); // Remember to add the RESET pin if your display requires it...

extern uint8_t frame_00 [];
extern uint8_t frame_01 [];
extern uint8_t frame_02 [];
extern uint8_t frame_03 [];
extern uint8_t frame_04 [];
extern uint8_t frame_05 [];
extern uint8_t frame_06 [];
extern uint8_t frame_07 [];
extern uint8_t frame_08 [];
extern uint8_t frame_09 [];
extern uint8_t frame_10 [];
extern uint8_t frame_11 [];
extern uint8_t frame_12 [];
extern uint8_t frame_13 [];
extern uint8_t uog[];
extern uint8_t SmallFont[];

const int pulsePin = 2;
int pulse = 0;
float power = 0;
float current = 0;
int volt = 0;
float pf = 0;

void setup()
{
  Serial.begin(115200);
  emon1.voltage(2, 103.2, 1.7);
  emon1.current(1, 4.6);
  attachInterrupt(digitalPinToInterrupt(pulsePin), pulseReading, RISING);
  if(!myOLED.begin(SSD1306_128X64))
    while(1);   // In case the library failed to allocate enough RAM for the display buffer...
  
  myOLED.setFont(SmallFont);

  myOLED.clrScr();
  myOLED.invert(true);
  myOLED.drawBitmap(0,0,uog,128,64);
  myOLED.update();
  delay(4000);
  myOLED.invert(false);
  myOLED.update();
  animation();
  animation();
  animation();
  animation();
}


void loop()
{ 
  emon1.calcVI(20,2000);
  
  power = emon1.realPower;
  power = abs(power);
  pf = emon1.powerFactor;
  pf = abs(pf);
  volt = emon1.Vrms;
  current = emon1.Irms;
  
  myOLED.clrScr();
  myOLED.print(String(pulse),0,0);
  myOLED.print(String(power,0),0,14);
  myOLED.print(String(pf,1),0,28);
  myOLED.print(String(volt),0,42);
  myOLED.print(String(current,1),0,54);
  myOLED.update();
}

void animation(){
  
  myOLED.clrScr();
  myOLED.print("PLEASE WAIT", 32,0);
  myOLED.drawBitmap(35, 14, frame_00, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_01, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_02, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_03, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_04, 64, 64); 
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_05, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_06, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_07, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_08, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_09, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_10, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_11, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_12, 64, 64);
  myOLED.update();
  delay(10);
  myOLED.drawBitmap(35, 14, frame_13, 64, 64);
  myOLED.update();
  delay(10);
}

void pulseReading(){
  pulse++;
  if(pulse >= 33){
    pulse = 1;
  }
}
