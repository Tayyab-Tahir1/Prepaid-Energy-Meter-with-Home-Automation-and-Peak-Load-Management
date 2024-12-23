#include <Wire.h>
#include "EmonLib.h"

EnergyMonitor emon1;

const int pulsePin = 2;
int pulse = 0;
int power = 0;
float current = 0;
int volt = 0;
float pf = 0;
float cons_kwh;
float rec_kwh;
float unitFactor = 0.03125;
int temp = 0; 

int32_t cons;
int32_t rec;
int pf_cos;
int amp;

byte myArray[17];
byte a,b,c,d,e,f,g,h;

void setup()
{
  Wire.begin(8);
  Serial.begin(115200);
  emon1.voltage(2, 103.2, 0);
  emon1.current(1, 4.6);
  attachInterrupt(digitalPinToInterrupt(pulsePin), pulseReading, RISING);
  
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

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
    if(rec_kwh > unitFactor){
      cons_kwh += unitFactor;
      rec_kwh -= unitFactor;
    }

    temp = pulse;
  }

}

void requestEvent(){
  int cx = cons_kwh;
  int rx = rec_kwh;
  cons = cons_kwh * pow(10,(5-(String(cx).length()-1)));
  rec = rec_kwh * pow(10,(5-(String(rx).length()-1)));
  /*Serial.print("cons: ");
  Serial.println(cons);*/
  Serial.print("rec: ");
  Serial.println(rec_kwh);
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

  myArray[8] = pf_cos & 0xFF;
  
  myArray[9] = (power >> 8) & 0xFF;
  myArray[10] = power & 0xFF;
  
  myArray[11] = (volt >> 8) & 0xFF;
  myArray[12] = volt & 0xFF;
  
  myArray[13] = (amp >> 8) & 0xFF;
  myArray[14] = amp & 0xFF;
  
  myArray[15] = (5-(String(cx).length()-1));
  myArray[16] = (5-(String(rx).length()-1));

  Wire.write(myArray, 17);
  
}

//external functions

void pulseReading(){
  pulse++;
  if(pulse >= 33){
    pulse = 1;
  }
}

void receiveEvent(int howMany){
  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();
  e = Wire.read();
  f = Wire.read();
  g = Wire.read();
  h = Wire.read();

  cons = a;
  cons = (cons << 8) | b;
  cons = (cons << 8) | c;
  cons = (cons << 8) | d;

  rec = e;
  rec = (rec << 8) | f;
  rec = (rec << 8) | g;
  rec = (rec << 8) | h;

  int cx2 = Wire.read();
  int rx2 = Wire.read();

  cons_kwh = cons/(1*pow(10,(5-cx2)));
  rec_kwh = rec/(1*pow(10,(5-rx2)));
  Serial.println(cons);
  Serial.println(rec_kwh);
  Serial.println(cx2);
  Serial.println(rx2);
  Serial.println();
}
