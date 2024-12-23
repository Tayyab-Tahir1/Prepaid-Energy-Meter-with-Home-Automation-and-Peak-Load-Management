#include <Wire.h>;
 
void setup()
{
Wire.begin(8);
Wire.onRequest(requestEvent);
}
 
void loop()
{
delay(100);
}
 
void requestEvent()
{
int32_t firstNum = 12345;
int32_t secondNum = 54321;
byte myArray[8];
 
myArray[0] = (firstNum >> 24) & 0xFF;
myArray[1] = (firstNum >> 16) & 0xFF;
myArray[2] = (firstNum >> 8) & 0xFF;
myArray[3] = firstNum & 0xFF;

myArray[4] = (secondNum >> 24) & 0xFF;
myArray[5] = (secondNum >> 16) & 0xFF;
myArray[6] = (secondNum >> 8) & 0xFF;
myArray[7] = secondNum & 0xFF;
 
Wire.write(myArray, 8);
}
