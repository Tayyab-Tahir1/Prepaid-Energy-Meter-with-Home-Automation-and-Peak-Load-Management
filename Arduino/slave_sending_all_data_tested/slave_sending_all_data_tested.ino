#include <Wire.h>
float cons_kwh = 0.99999;
float rec_kwh = 0.99999;
float pf = 0.85;
int power = 500;
int volt = 226;
float current = 1.88;

int32_t cons;
int32_t rec;
int pf_cos;
int amp;

byte myArray[12];

void setup(){
  Wire.begin(8);
  Serial.begin(115200);
  Wire.onRequest(requestEvent);
}

void loop(){
  
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
