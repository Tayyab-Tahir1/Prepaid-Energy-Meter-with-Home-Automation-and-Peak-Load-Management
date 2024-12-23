#include <Wire.h>
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
extern uint8_t TinyFont[];

const int pulsePin = 2;
int pulse = 0;
int power = 0;
float current = 0;
int volt = 0;
float pf = 0;
float cons_kwh = 0;
float rec_kwh = 11;
float unitFactor = 0.03125;
int temp = 0; 

int32_t cons;
int32_t rec;
int pf_cos;
int amp;

byte myArray[12];

void setup()
{
  Wire.begin(8);
  Serial.begin(115200);
  emon1.voltage(2, 103.2, 0);
  emon1.current(1, 4.6);
  attachInterrupt(digitalPinToInterrupt(pulsePin), pulseReading, RISING);
  if(!myOLED.begin(SSD1306_128X64))
    while(1);   // In case the library failed to allocate enough RAM for the display buffer...
  
  Wire.onRequest(requestEvent);
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

  if(pulse != temp){
    cons_kwh += unitFactor;
    rec_kwh -= unitFactor;

    temp = pulse;
  }

  if(rec_kwh <= 0){
    myOLED.clrScr();
    myOLED.setFont(SmallFont);
    myOLED.print("LOW BALANCE",34,24);
    myOLED.print("PLEASE RECHARGE",23,39);
    myOLED.update();
  }
  else{
    nor_display();
  }

}

void requestEvent(){
  int cx = cons_kwh;
  int rx = rec_kwh;
  cons = cons_kwh * pow(10,(5-(String(cx).length()-1)));
  rec = rec_kwh * pow(10,(5-(String(rx).length()-1)));
  Serial.print("cons: ");
  Serial.println(cons);
  Serial.print("rec: ");
  Serial.println(rec);
  Serial.println();
  pf_cos = pf * 100;
  amp = current * 100;

  myArray[0] = (cons >> 24) & 0xFF;
  myArray[1] = (cons >> 16) & 0xFF;
  myArray[2] = (cons >> 8) & 0xFF;
  myArray[3] = cons & 0xFF;

  myArray[4] = (rec >> 24) & 0xFF;
  myArray[5] = (rec >> 16) & 0xFF;
  myArray[6] = (rec >> 8) & 0xFF;
  myArray[7] = rec & 0xFF;

  myArray[8] = pf_cos;
  myArray[9] = power;
  myArray[10] = volt;
  myArray[11] = amp;

  Wire.write(myArray, 12);
  
}



//external functions
void nor_display(){
  int x = cons_kwh;
  int y = rec_kwh;
  myOLED.clrScr();
  myOLED.setFont(TinyFont);
  myOLED.print("SMART PREPAID ENERGY METER", 12,0);
  myOLED.print("__________________________________", 0,6);
  myOLED.setFont(SmallFont);
  myOLED.print("Consum KWh :", 4,16);
  myOLED.print(String(cons_kwh,(5-(String(x).length()-1))), 79,16);
  myOLED.print("Remain KWh :", 4,29);
  myOLED.print(String(rec_kwh,(5-(String(y).length()-1))), 79,29);
  myOLED.print("Volt:", 0,42);
  myOLED.print(String(volt), 33,42);   //replace "230" with PT variable
  myOLED.print("PF:", 79,42);
  if(current <= 0.04){
    myOLED.print("   ", 110,42);
  }
  else{
    myOLED.print(String(pf,2), 99,42);
  }
  myOLED.setFont(TinyFont);
  myOLED.print("__________________________________",0,51);
  myOLED.print("CONSUMER ID : 18005019007", 12,59);
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
