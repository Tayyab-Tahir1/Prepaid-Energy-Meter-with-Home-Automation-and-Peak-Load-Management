#include <Wire.h>
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);

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

int32_t cons, rec;
byte a,b,c,d,e,f,g,h;
int pf_cos, power, volt, current, cx, rx;
float cons_kwh, rec_kwh, pf,Irms;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  if(!myOLED.begin(SSD1306_128X64))
    while(1);

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

void loop(){
  delay(1000);
  Wire.requestFrom(8,14);

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

  pf_cos = Wire.read();
  power = Wire.read();
  volt = Wire.read();
  current = Wire.read();
  cx = Wire.read();
  rx = Wire.read();

  cons_kwh = cons/(1*pow(10,cx));
  rec_kwh = rec/(1*pow(10,rx));
  pf = pf_cos/100;
  Irms = current/100;

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
  Serial.println(Irms);
  Serial.println();
}

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
