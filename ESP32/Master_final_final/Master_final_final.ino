#include <WiFi.h>
#include <Wire.h>
#include <math.h>
#include "time.h"
#include <OLED_I2C.h>

OLED  myOLED(SDA, SCL);

#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://smartmeter-e9738-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "j2kJRCDuE95uMEbmqdrutpXWzv2i7fTX0x7CwqZ1"
FirebaseData firebaseData;

const char* ntpServer = "pk.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

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
float amount, rate;
String amount_str;
int time_data;
int peak_start;
int peak_stop;

//pins and temp variables
int resetPin = 23;
int power_load = 2;
int load2 = 4;
int load3 = 27;
int load4 = 26;

float temp_cons = -1;
float temp_rec = -1;
float temp_Irms = -1;
float temp_pf = -1;
int temp_volt = -1;
int temp_power = -1;

void setup(){
  Wire.begin();
  Serial.begin(115200);
  WiFi.disconnect();
  pinMode(resetPin, OUTPUT);
  pinMode(power_load, OUTPUT);
  pinMode(load2, OUTPUT);
  pinMode(load3, OUTPUT);
  pinMode(load4, OUTPUT);
  digitalWrite(power_load, LOW);
  digitalWrite(load2, LOW);
  digitalWrite(load3, LOW);
  digitalWrite(load4, LOW);

  digitalWrite(resetPin, HIGH);
  if(!myOLED.begin(SSD1306_128X64))
    while(1);
  myOLED.setFont(SmallFont);
  myOLED.clrScr();
  myOLED.invert(true);
  myOLED.drawBitmap(0,0,uog,128,64);
  myOLED.update();
  delay(4000);
  //delay(1000);
  Serial.println("START");
  WiFi.begin("Wi_Fi","PaK@$%6141304");
  while ((!(WiFi.status() == WL_CONNECTED))){
    //delay(300);
    Serial.print("..");
    //Serial.println((WiFi.status() == WL_CONNECTED));
    for(int aaa = 0; aaa<=10; aaa++){
      animation();
      if((WiFi.status() == WL_CONNECTED) == 1){
        digitalWrite(resetPin, LOW);
        break;
      }
    }
  }

  //Serial.println((WiFi.status() == WL_CONNECTED));
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (Firebase.getString(firebaseData, "/amount")) {
    amount_str = firebaseData.stringData();
    }
  if (Firebase.getFloat(firebaseData, "/rate")) {
    rate = firebaseData.floatData();
    }
  if (Firebase.getFloat(firebaseData, "/rec_kwh")) {
    rec_kwh = firebaseData.floatData();
    }
  if (Firebase.getFloat(firebaseData, "/cons_kwh")) {
    cons_kwh = firebaseData.floatData();
    }
  if (Firebase.getInt(firebaseData, "/peak_start")) {
    peak_start = firebaseData.intData();
    }
  if (Firebase.getInt(firebaseData, "/peak_stop")) {
    peak_stop = firebaseData.intData();
    }
    
  amount = amount_str.toFloat();
  if(amount>0){
    rec_kwh = rec_kwh + amount/rate;
  }
  Serial.println("amount: ");
  Serial.println(amount);
  if ((Firebase.setString(firebaseData, "/amount", "0")) == true) {
      //Serial.println("Import Energy Posted");
     }
 
  sendData();
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

//NTP server added
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  int time_data = timeinfo.tm_hour + 3;

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

//data posting
  if(temp_cons != cons_kwh){
    if ((Firebase.setFloat(firebaseData, "/cons_kwh", cons_kwh)) == true) {
      Serial.println("Consumed Energy Posted");
      }
    temp_cons = cons_kwh;
  }
  
  if(temp_rec != rec_kwh){
    if ((Firebase.setFloat(firebaseData, "/rec_kwh", rec_kwh)) == true) {
      Serial.println("Remaining Energy Posted");
      }
    temp_rec = rec_kwh;
  }

  if(Irms <= 0.04){
    pf = 0;
  }
  if(temp_pf != pf){
    if ((Firebase.setFloat(firebaseData, "/pf", pf)) == true) {
      Serial.println("Power factor Posted");
      }
    temp_pf = pf;
  }

  if(temp_power != power){
    if ((Firebase.setInt(firebaseData, "/watt", power)) == true) {
      Serial.println("Load power Posted");
      }
    temp_power = power;
  }

  if(temp_volt != volt){
    if ((Firebase.setInt(firebaseData, "/volt", volt)) == true) {
      Serial.println("Supply voltage Posted");
      }
    temp_volt = volt;
  }

  if(temp_Irms != Irms){
    if ((Firebase.setFloat(firebaseData, "/current", Irms)) == true) {
      Serial.println("Current Posted");
      }
    temp_Irms = Irms;
  }

// time checking
  if (Firebase.getInt(firebaseData, "/peak_start")) {
    peak_start = firebaseData.intData();
    }
  if (Firebase.getInt(firebaseData, "/peak_stop")) {
    peak_stop = firebaseData.intData();
    }

//load switching
  if (Firebase.getString(firebaseData, "/PS1")) {
    if((firebaseData.stringData()) == "1"){
      if(time_data >= peak_start && time_data <= peak_stop){
        digitalWrite(power_load, LOW);
      }
      else{
        if(rec_kwh <= 0){
          digitalWrite(power_load, LOW);
        }
        else{
          digitalWrite(power_load, HIGH);
        }
      }
    }
     if((firebaseData.stringData()) == "0"){
      digitalWrite(power_load, LOW);
    }
   }

  if (Firebase.getString(firebaseData, "/S1")) {
    if((firebaseData.stringData()) == "1"){
      if(rec_kwh <= 0){
          digitalWrite(load2, LOW);
        }
        else{
          digitalWrite(load2, HIGH);
        }
      }
     if((firebaseData.stringData()) == "0"){
      digitalWrite(load2, LOW);
    }
   }

  if (Firebase.getString(firebaseData, "/S2")) {
    if((firebaseData.stringData()) == "1"){
      if(rec_kwh <= 0){
          digitalWrite(load3, LOW);
        }
        else{
          digitalWrite(load3, HIGH);
        }
    }
     if((firebaseData.stringData()) == "0"){
      digitalWrite(load3, LOW);
    }
   }

  if (Firebase.getString(firebaseData, "/S3")) {
    if((firebaseData.stringData()) == "1"){
      if(rec_kwh <= 0){
          digitalWrite(load4, LOW);
        }
        else{
          digitalWrite(load4, HIGH);
        }
    }
     if((firebaseData.stringData()) == "0"){
      digitalWrite(load4, LOW);
    }
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
  Serial.print("Time hour: ");
  Serial.println(time_data);
  Serial.println();

  if (Firebase.getString(firebaseData, "/amount")) {
      amount_str = firebaseData.stringData();
    }
   if(amount_str != "0"){
    ESP.restart();
   }

    
}

void sendData(){
  byte sendArray[10];  
  int cx2 = cons_kwh;
  int rx2 = rec_kwh;
  cx2 = String(cx2).length()-1;
  rx2 = String(rx2).length()-1;
  cons = cons_kwh * pow(10,(5-cx2));
  rec = rec_kwh * pow(10,(5-rx2));          //replace 2.0 with firebase import

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
  //myOLED.print("CONSUMER ID : 18143122027", 12,59);
  myOLED.update();
}

void animation(){
  myOLED.invert(false);
  myOLED.update();
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
