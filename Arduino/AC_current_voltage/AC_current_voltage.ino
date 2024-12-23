#include "EmonLib.h"
EnergyMonitor emon1;

void setup(){
  Serial.begin(115200);
  emon1.voltage(2, 103.2, 1.7);
  emon1.current(1, 4.1);
}

void loop(){
  emon1.calcVI(20,2000);

  float realPower       = emon1.realPower;        //extract Real Power into variable
  float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
  float powerFactor     = emon1.powerFactor;      //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             //extract Vrms into Variable
  float Irms            = emon1.Irms;

  Serial.print("Current: ");
  Serial.println(Irms, 1);
  Serial.print("\n\n\Voltage: ");
  Serial.println(supplyVoltage, 0);
  delay(500);
}
