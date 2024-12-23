#include <Wire.h>
#include <math.h>
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
byte a,b,c,d,e,f,g,h,i,j,k,l,m,n,o;
byte myArray[10];
int16_t watt, volt_supply, cx, rx;
int power, volt, pf_cos, current;
float cons_kwh, rec_kwh;
float Irms;
float pf;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  if(!myOLED.begin(SSD1306_128X64))
    while(1);

  sendData();
  
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
  Wire.requestFrom(8,17);

  a = Wire.read();
  b = Wire.read();
  c = Wire.read();
  d = Wire.read();
  e = Wire.read();
  f = Wire.read();
  g = Wire.read();
  h = Wire.read();
  i = Wire.read();
  j = Wire.read();
  k = Wire.read();
  l = Wire.read();
  m = Wire.read();
  n = Wire.read();
  o = Wire.read();
  
  cons = a;
  cons = (cons << 8) | b;
  cons = (cons << 8) | c;
  cons = (cons << 8) | d;

  rec = e;
  rec = (rec << 8) | f;
  rec = (rec << 8) | g;
  rec = (rec << 8) | h;

  pf_cos = i;

  watt = j;
  watt = (watt << 8) | k;

  volt_supply = l;
  volt_supply = (volt_supply << 8) | m;

  current = n;
  current = (current << 8) | o;

  cx = Wire.read();
  rx = Wire.read();

  //pf = pf_cos/100;
  power = watt;
  volt = volt_supply;
  //Irms = current/100;
  

  cons_kwh = cons/(1*pow(10,cx));
  rec_kwh = rec/(1*pow(10,rx));
  pf = (float(pf_cos)/100);
  Irms = (float(current)/100);

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

void sendData(){
  byte sendArray[10];
  int cx2 = cons_kwh;
  int rx2 = rec_kwh;
  cx2 = String(cx2).length()-1;
  rx2 = String(rx2).length()-1;
  cons = cons_kwh * pow(10,(5-cx2));
  rec = 2.0 * pow(10,(5-rx2));          //replace 2.0 with firebase import

  myArray[0] = (cons >> 24) & 0xFF;
  myArray[1] = (cons >> 16) & 0xFF;
  myArray[2] = (cons >> 8) & 0xFF;
  myArray[3] = cons & 0xFF;

  myArray[4] = (rec >> 24) & 0xFF;
  myArray[5] = (rec >> 16) & 0xFF;
  myArray[6] = (rec >> 8) & 0xFF;
  myArray[7] = rec & 0xFF;

  myArray[8] = cx2;
  myArray[9] = rx2;
  
  Wire.beginTransmission(8);
  Wire.write(myArray, 10);
  Wire.endTransmission();
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
  if(Irms <= 0.04){
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
