#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// defining input pins
const int solarPin = A0;
const int batteryPin = A1;
const int wapdaPin = A2;
const int loadPin = A3;
const int signalPin = 7;

// defining sensitivity 185 for 5Amp, 100 for 20Amp and 66 for 30Amp
int sensitivity = 100;

// defining adc values 
int solar_adc_value= 0;
int battery_adc_value= 0;
int wapda_adc_value= 0;
int load_adc_value= 0;

// defining adc voltages
double solar_adc_voltage = 0;
double battery_adc_voltage = 0;
double wapda_adc_voltage = 0;
double load_adc_voltage = 0;

// defining current variables
double solarCurrent = 0;
double batteryCurrent = 0;
double wapdaCurrent = 0;
double loadCurrent = 0;

// defining standard offset voltages
int offsetVoltage = 2500;
 
void setup() 
{
  pinMode(solarPin, INPUT);
  pinMode(batteryPin, INPUT);
  pinMode(wapdaPin, INPUT);
  pinMode(loadPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 4);
}
 
void loop()
{
  // calculating all adc values
  solar_adc_value = analogRead(solarPin);
  battery_adc_value = analogRead(batteryPin);
  wapda_adc_value = analogRead(wapdaPin);
  load_adc_value = analogRead(loadPin);

  // calculating all adc voltages
  solar_adc_voltage = (solar_adc_value / 1024.0) * 5000;
  battery_adc_voltage = (battery_adc_value / 1024.0) * 5000;
  wapda_adc_voltage = (wapda_adc_value / 1024.0) * 5000;
  load_adc_voltage = (load_adc_value / 1024.0) * 5000;

  // calculating all currents
  solarCurrent = ((solar_adc_voltage - offsetVoltage) / sensitivity);
  batteryCurrent = ((battery_adc_voltage - offsetVoltage) / sensitivity);
  wapdaCurrent = ((wapda_adc_voltage - offsetVoltage) / sensitivity);
  loadCurrent = ((load_adc_voltage - offsetVoltage) / sensitivity);
  
  
  lcd.setCursor(0,0);
  lcd.print("S:");
  lcd.print(solarCurrent,1);
  //lcd.print(" ");
  lcd.print("W:");
  lcd.print(abs(wapdaCurrent),1);
  //lcd.print(" ");
  lcd.print("L:");
  lcd.print(loadCurrent,1);
  //lcd.print(" ");
  if(wapdaCurrent<0){
    lcd.setCursor(0,1);
    lcd.print("Power Exporting");
    digitalWrite(signalPin, HIGH);
  }
  if(wapdaCurrent>0){
    lcd.setCursor(0,1);
    lcd.print("Power Importing");
    digitalWrite(signalPin, LOW);
  }
  if(wapdaCurrent==0){
    lcd.setCursor(0,1);
    lcd.print("Grid Disconnect");
    digitalWrite(signalPin, LOW);
  }
  if(batteryCurrent>0){
    lcd.setCursor(0,2);
    lcd.print("Battery Disch...");
  }
  if(batteryCurrent<0){
    lcd.setCursor(0,2);
    lcd.print("Battery Charging");
  }
  if(batteryCurrent==0){
    lcd.setCursor(0,2);
    lcd.print("                ");
  }
  lcd.setCursor(0,3);
  lcd.print(" Bi-Directional ");
  lcd.display();
  /*Serial.print("Solar current = " );
  Serial.println(solarCurrent, 2);
  Serial.println("");     //just for a line gap
  Serial.print("Battery current = ");
  Serial.println(batteryCurrent,2);
  Serial.println("");     //just for a line gap
  Serial.print("WAPDA current = ");
  Serial.println(wapdaCurrent,2);
  Serial.println("");     //just for a line gap
  Serial.print("Load current = ");
  Serial.println(loadCurrent,2);
  Serial.println("");     //just for a line gap
  delay(1000);*/
}
