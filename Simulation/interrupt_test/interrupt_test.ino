#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pulsePin = 2;
const int signalPin = 10;
const int gridSignal = 9;

int pin = A0;
float rads = 57.29577951; // 1 radian = approx 57 deg.
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6); // Multiplication factor to convert nano seconds into seconds

// Define floats to contain calculations

float pf;
float angle;
float pf_max = 0;
float angle_max = 0;
int ctr;

int signalValue = 0;
int gridValue = 0;
int pulse = 0;
float expKwh = 0;
float impKwh = 0;
int temp = 0; 

void setup(){
  Serial.begin(9600);
  lcd.begin(16,4);
  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pulsePin), pulseReading, RISING);
}

void loop(){
  signalValue = digitalRead(signalPin);
  gridValue = digitalRead(gridSignal);

  lcd.setCursor(0,0);
  lcd.print("Export kWH:");
  lcd.setCursor(0,1);
  lcd.print("Import kWH:");
  if(pulse!=temp){
    if(signalValue==HIGH){
      expKwh = expKwh + 0.03;
      lcd.setCursor(12,0);
      lcd.print(expKwh,2);
    }
  
    if(signalValue==LOW){
      impKwh = impKwh + 0.03;
      lcd.setCursor(12,1);
      lcd.print(impKwh,2);
    }
    temp = pulse;
  }
  lcd.display();

  //powerFactor
  for (ctr = 0; ctr <= 4; ctr++) // Perform 4 measurements then reset
  {
  // 1st line calculates the phase angle in degrees from differentiated time pulse
  // Function COS uses radians not Degree's hence conversion made by dividing angle / 57.2958
   angle = ((((pulseIn(pin, HIGH)) * nano)* degree)* frequency);
  // pf = cos(angle / rads); 
   
   if (angle > angle_max) // Test if the angle is maximum angle
     {
      angle_max = angle; // If maximum record in variable "angle_max"
      pf_max = cos(angle_max / rads); // Calc PF from "angle_max"
     }
   }
   if (angle_max > 360) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   lcd.setCursor(0,2);
   lcd.print("PowerFactor:");
   lcd.print(pf_max,2);
  
 
  //delay(1000);
}

void pulseReading(){
  pulse++;
  if(pulse >= 33){
    pulse = 1;
  }
   
}
