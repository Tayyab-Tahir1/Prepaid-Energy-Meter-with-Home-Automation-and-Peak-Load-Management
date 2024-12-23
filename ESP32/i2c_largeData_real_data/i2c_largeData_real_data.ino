#include <Wire.h>

int32_t cons, rec;
byte a,b,c,d,e,f,g,h;
int pf, power, volt, current;

void setup(){
  Wire.begin();
  Serial.begin(115200);
}

void loop(){
  delay(1000);
  Wire.requestFrom(8,12);

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

  pf = Wire.read();
  power = Wire.read();
  volt = Wire.read();
  current = Wire.read();

  Serial.print("cons: ");
  Serial.println(cons);
  Serial.print("rec: ");
  Serial.println(rec);
  Serial.print("PF: ");
  Serial.println(pf);
  Serial.print("Watt: ");
  Serial.println(power);
  Serial.print("Volt: ");
  Serial.println(volt);
  Serial.print("Current: ");
  Serial.println(current);
  Serial.println();
}
